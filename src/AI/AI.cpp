// implements AI.h

#include "AI.h"

#include "tt.h"

#include <vector>
#include <cassert>
#include <algorithm>
#include <utility>

constexpr const int SVALUE_INF = 1 << 14;
constexpr const int SFORCED_WIN = 1 << 13;
constexpr const int SFORCED_LOSS = -SFORCED_WIN;

AI::AI(const timestamp _avail_time, const Board& _current_board, Board& _result, std::atomic <bool>& _is_done) :
m_avail_time(_avail_time),
m_is_done(_is_done)
{
	TT.clear();
	
	// reset timer on construction
	m_timer = Timer();
	
	// iterative deepening scope
	// 'depth < 1000' exists only for making sure some bug doesn't send the for loop into millions of iterations
	for (int depth = 1; !_is_done &&
	!(m_timed_out |= m_timer.current() >= _avail_time) && depth < 1000; depth++) {
		int local_result = m_minmax(_current_board, -SVALUE_INF, SVALUE_INF, depth);
		if (m_is_done || m_timed_out) {
			break;
		}
		// only update the best move if the previous search finished
		m_best_move = m_current_best_move;
		if (abs(local_result) >= SFORCED_WIN - 7 * 6 - 1) {
			break;
		}
	}
	
	// make the best move
	assert(m_best_move != -1);
	_result = _current_board;
	_result.make_move(m_best_move);
	
	// set is_done flag when ready to be joined
	_is_done = true;
}

int AI::m_minmax(Board board, int alpha, int beta, int depth, int from_root) {
	if (m_is_done || m_timed_out || (m_timed_out |= m_timer.current() >= m_avail_time)) {
		return 0;
	}
	
	// if a previously found forced win is possible in fewer moves than the current depth, return
	if (from_root) {
		alpha = std::max(alpha, SFORCED_LOSS + from_root);
		beta = std::min(beta, SFORCED_WIN - from_root);
		if (alpha >= beta) {
			return alpha;
		}
	}
	
	// if an entry in the transposition table is available: use that...
	TT_entry* tt_entry = TT.query(depth, board.get_mask(), alpha, beta);
	if (tt_entry) {
		return tt_entry->value;
	}
	
	// if our opponent won with their last move...
	if (bb::has_won(board.get_board(true))) {
		return SFORCED_LOSS + from_root;
	}
	
	// if there are no moves left it is a draw
	if (!bb::moves_left(board.get_both())) {
		return 0;
	}
	
	// if we reached maximum depth, return static eval
	if (!depth) {
		return m_static_eval(board);
	}
	
	// find all legal moves
	std::vector <int> moves;
	moves.reserve(7);
	for (int i = 0; i < 7; i++) {
		if (board.is_legal(i)) {
			moves.push_back(i);
		}
	}
	
	// sort the legal moves
	// best move ordering is assumed to be moves closest to the center, for now
	std::sort(moves.begin(), moves.end(), [] (int u, int v) { return abs(3 - u) < abs(3 - v); });
	
	uint64_t bound = BOUND_UPPER;
	
	// try all moves...
	for (int col : moves) {
		// stop if we timed out
		if (m_timed_out) {
			break;
		}
		
		// recursive call
		Board nxt = board;
		nxt.make_move(col);
		int value = -m_minmax(nxt, -beta, -alpha, depth - 1, from_root + 1);
		
		// beta cutoff ?
		if (value >= beta) {
			TT.update(depth, beta, board.get_mask(), BOUND_LOWER, board.get_turn());
			return beta;
		}
		
		// new best ?
		if (value > alpha) {
			alpha = value;
			// update the current best move if alpha value was beating at root node
			if (!from_root) {
				m_current_best_move = col;
			}
		}
	}
	
	TT.update(depth, alpha, board.get_mask(), bound, board.get_turn());
	
	return alpha;
}

int AI::m_static_eval(Board board) {
	int result = 0;
	
	bit_board b;
	bit_board b2;
	bit_board b3;
	
	bit_board mboard = board.get_board();
	bit_board other = board.get_board(true);
	
	// compute the value for the current player and accumulate the value
	// do the same for opposing player, but multiply by -1 when accumulating
	for (const int mul : { 1, -1 }) {
		int local_result = 0;
		
		// give a bonus for connect 2
		// give the bonus once for every possible way to complete the connect 4
		{
			// vertical
			b = mboard & (mboard >> 1);
			b2 = bb::full_board & (bb::full_board >> 1);
			b &= b2 >> 2;
			local_result += __builtin_popcountll(b);
			// horizontal
			for (int i = 0; i <= 2; i++) {
				bit_board initial = mboard;
				if (i > 0) initial = (initial >> 7) & ~other;
				if (i > 1) initial = (initial >> 7) & ~other;
				b = initial & (initial >> 7);
				b2 = bb::full_board & (bb::full_board >> 7);
				b &= b2 >> 14;
				local_result += __builtin_popcountll(b);
			}
			// right diagonal
			for (int i = 0; i <= 2; i++) {
				bit_board initial = mboard;
				if (i > 0) initial = (initial >> 8) & ~other;
				if (i > 1) initial = (initial >> 8) & ~other;
				b = initial & (initial >> 8);
				b2 = bb::full_board & (bb::full_board >> 8);
				b &= b2 >> 16;
				local_result += __builtin_popcountll(b);
			}
			// left diagonal
			for (int i = 0; i <= 2; i++) {
				bit_board initial = mboard;
				if (i > 0) initial = (initial >> 6) & ~other;
				if (i > 1) initial = (initial >> 6) & ~other;
				b = initial & (initial >> 6);
				b2 = bb::full_board & (bb::full_board >> 6);
				b &= b2 >> 12;
				local_result += __builtin_popcountll(b);
			}
		}
		// give a bonus for connect 3
		// give the bonus once for every possible way to complete the connect 4
		{
			// vertical
			b = mboard & (mboard >> 1) & (mboard >> 2);
			b2 = bb::full_board & (bb::full_board >> 1);
			b &= b2 >> 2;
			local_result += 4 * __builtin_popcountll(b);
			// horizontal
			b = mboard & (mboard >> 7) & (mboard >> 14);
			b3 = (mboard >> 7) & ~other;
			b3 = b3 & (b3 >> 7) & (b3 >> 14);
			b2 = bb::full_board & (bb::full_board >> 7);
			b &= b2 >> 7;
			b3 &= b2 >> 7;
			local_result += 4 * __builtin_popcountll(b);
			local_result += 4 * __builtin_popcountll(b3);
			// right diagonal
			b = mboard & (mboard >> 8) & (mboard >> 16);
			b3 = (mboard >> 8) & ~other;
			b3 = b3 & (b3 >> 8) & (b3 >> 16);
			b2 = bb::full_board & (bb::full_board >> 8);
			b &= b2 >> 8;
			b3 &= b2 >> 8;
			local_result += 4 * __builtin_popcountll(b);
			local_result += 4 * __builtin_popcountll(b3);
			// left diagonal
			b = mboard & (mboard >> 6) & (mboard >> 12);
			b3 = (mboard >> 6) & ~other;
			b3 = b3 & (b3 >> 6) & (b3 >> 12);
			b2 = bb::full_board & (bb::full_board >> 6);
			b &= b2 >> 6;
			b3 &= b2 >> 6;
			local_result += 4 * __builtin_popcountll(b);
			local_result += 4 * __builtin_popcountll(b3);
		}
		// give a bonus for every tile close to the middle
		local_result += 2 * __builtin_popcountll(mboard & bb::col[3]);
		local_result += __builtin_popcountll(mboard & (bb::col[2] | bb::col[4]));
		
		// add the accumulated local result to the final result
		result += local_result * mul;
		
		// make a null move to prepare for calculating score for the opposing player
		std::swap(mboard, other);
	}
	
	return result;
}
