// 6x7 bit board utility

// each column is 7 continous bits, most significant 1 bit is the top of the stack (itself empty)
// thus 49 bits
// most functions consider bitboards without the above layout, simply with some set tiles and some unset tiles

#pragma once

#include <iostream>

typedef unsigned long long bit_board;

namespace bb {
	
	constexpr const bit_board bottom_row = (1LL << (7 * 0)) |
	(1LL << (7 * 1)) | (1LL << (7 * 2)) | (1LL << (7 * 3)) |
	(1LL << (7 * 4)) | (1LL << (7 * 5)) | (1LL << (7 * 6));
	
	// every bit in col[i] is on, including the square just above the board
	constexpr const bit_board col[7] = {
		((1LL << 7) - 1) << (7 * 0),
		((1LL << 7) - 1) << (7 * 1),
		((1LL << 7) - 1) << (7 * 2),
		((1LL << 7) - 1) << (7 * 3),
		((1LL << 7) - 1) << (7 * 4),
		((1LL << 7) - 1) << (7 * 5),
		((1LL << 7) - 1) << (7 * 6)
	};
	
	// every bit in row[i] is on, row[6] has all bits on just above the board
	constexpr const bit_board row[7] = {
		bottom_row << 0,
		bottom_row << 1,
		bottom_row << 2,
		bottom_row << 3,
		bottom_row << 4,
		bottom_row << 5,
		bottom_row << 6
	};
	
	constexpr const bit_board full_board = row[0] | row[1] | row[2] | row[3] | row[4] | row[5];
	
	// return true if the passed board is winning for the player with 1 bits
	inline bool has_won(const bit_board board) {
		bit_board b;
		
		// vertical check
		b = board & (board >> 1);
		if ((b >> 2) & b) {
			return true;
		}
		
		// horizontal check
		b = board & (board >> 7);
		if ((b >> 14) & b) {
			return true;
		}
		
		// right diagonal
		b = board & (board >> 8);
		if ((b >> 16) & b) {
			return true;
		}
		
		// left diagonal
		b = board & (board >> 6);
		if ((b >> 12) & b) {
			return true;
		}
		
		return false;
	}
	
	// returns true if there is at least one move left
	constexpr bool moves_left(const bit_board both) {
		return both ^ full_board;
	}
	
	inline void print_board(std::ostream& stream, const bit_board board) {
		stream << "+---------------+\n";
		for (int j = 6; j >= 0; j--) {
			stream << "| ";
			for (int i = 0; i <= 6; i++) {
				stream << ((board >> (i * 7 + j)) & 1) << " ";
			}
			stream << "|\n";
		}
		stream << "+---------------+\n";
	}
	
} /// namespace bb

class Board {
	
public:
	
	constexpr Board() :
	m_board(static_cast <bit_board> (0)),
	m_both(static_cast <bit_board> (0)),
	m_turn(0)
	{ }
	
	inline void make_move(int col) {
		m_board ^= m_both;
		m_both |= m_both + (bb::col[col] & bb::row[0]);
		m_turn ^= 1;
	}
	
	template <bool FIX_TURN = false> constexpr bit_board get_mask() const {
		return (((m_both + bb::row[0]) | m_both) ^ m_both) |
		(m_board ^ (FIX_TURN && m_turn ? m_both : static_cast <bit_board> (0)));
	}
	
	constexpr bit_board get_board(bool reverse_turn = false) const {
		return m_board ^ (reverse_turn ? m_both : static_cast <bit_board> (0));
	}
	
	constexpr bit_board get_both() const {
		return m_both;
	}
	
	constexpr bool get_turn() const {
		return m_turn;
	}
	
	// returns true if a given move is legal
	constexpr bool is_legal(int col) const {
		return !((bb::row[5] & bb::col[col]) & m_both);
	}
	
	// make a null move / flip active ones and zeroes
	inline void flip() {
		m_board ^= m_both;
	}
	
private:
	
	bit_board m_board;
	bit_board m_both;
	bool m_turn;
	
};
