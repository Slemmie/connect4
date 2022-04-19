// AI entry point

#pragma once

#include "../game/bit_board.h"
#include "../util/timer.h"

#include <atomic>

class AI {
	
public:
	
	AI(const timestamp _avail_time, const Board& _current_board, Board& _result, std::atomic <bool>& _is_done);
	
private:
	
	Timer m_timer;
	
	timestamp m_avail_time;
	
	bool m_timed_out = false;
	
	int m_best_move = -1;
	int m_current_best_move = -1;
	
	std::atomic <bool>& m_is_done;
	
private:
	
	int m_minmax(Board board, int alpha, int beta, int depth, int from_root = 0);
	
	int m_static_eval(Board board);
	
};
