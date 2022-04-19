// implements game.h

#include "game.h"

#include "../graphics/graphics.h"

#include <iostream>

Game::Game(bool _is_one_player) :
m_is_one_player(_is_one_player)
{
	m_winning_tiles[0] = 0;
	m_winning_tiles[1] = 0;
	m_winning_tiles[2] = 0;
	m_winning_tiles[3] = 0;
	
	// use the menu background shader as our background as well
	m_background_shader = std::make_shared <gp::Shader> ();
	m_background_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/menu/menu_vertex.glsl");
	m_background_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/menu/menu_fragment.glsl");
	m_background_shader->assemble();
	
	m_background_rect = std::make_unique <gp::Rect> (m_background_shader, 1, 0, 1, 0);
	
	m_board_shader = std::make_shared <gp::Shader> ();
	m_board_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/game/board_vertex.glsl");
	m_board_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/game/board_fragment.glsl");
	m_board_shader->assemble();
	
	m_board_rect = std::make_unique <gp::Rect> (m_board_shader, nullptr, 1, 0, 1, 0);
	m_board_rect->make_square();
	
	// set mouse press callback
	gp::push_callback_mouse_press([this] (int button, double x, double y) ->
	void { this->m_mouse_press(button, x, y); });
}

extern Application_mode APP_MODE;

bool Game::frame() {
	m_background_rect->render();
	
	// set the current board as a uniform variable
	m_board_shader->bind();
	unsigned int col0123 = static_cast <unsigned int>
	( m_board.get_mask <true> () & (bb::col[0] | bb::col[1] | bb::col[2] | bb::col[3]));
	unsigned int col456  = static_cast <unsigned int>
	((m_board.get_mask <true> () & (bb::col[4] | bb::col[5] | bb::col[6])) >> (7 * 4));
	m_board_shader->set_uniform_1ui("u_col0123", col0123);
	m_board_shader->set_uniform_1ui("u_col456", col456);
	// set a uniform telling where to potentially highlight
	if ((m_board.get_turn() && m_is_one_player) || APP_MODE != APP_MODE_GAME) {
		m_board_shader->set_uniform_1ui("u_hover_col", 0);
		m_board_shader->set_uniform_1ui("u_turn", 2);
	} else {
		m_board_shader->set_uniform_1ui("u_hover_col", m_which_hover());
		m_board_shader->set_uniform_1ui("u_turn", m_board.get_turn());
	}
	// if the game is over, send the four winning tiles to the shader ... if they exist
	// if the game is not over, or if the result was a tie, four zeroes are sent to the shader
	m_board_shader->set_uniform_4ui("u_winning_tiles",
	m_winning_tiles[0], m_winning_tiles[1], m_winning_tiles[2], m_winning_tiles[3]);
	
	m_board_shader->unbind();
	
	// render the board
	m_board_rect->render();
	
	// if the game is over, return before checking for game over
	if (APP_MODE == APP_MODE_OVER) {
		return true;
	}
	
	// check if the game is over
	if (bb::has_won(m_board.get_board()) ||
	bb::has_won(m_board.get_board(true)) ||
	!bb::moves_left(m_board.get_both())) {
		APP_MODE = APP_MODE_OVER;
		
		m_board_shader->bind();
		m_board_shader->set_uniform_1f("u_game_over_timestamp", gp::get_time());
		m_board_shader->unbind();
		
		// once, do a greedy check for which tiles are the winning ones
		for (int rev = 0; rev <= 1; rev++) {
			bit_board b;
			bit_board board = m_board.get_board(rev);
			
			// vertical check
			b = board & (board >> 1);
			b = (b >> 2) & b;
			if (b) {
				m_winning_tiles[0] = __builtin_ctzll(b);
				m_winning_tiles[1] = __builtin_ctzll(b) + 1;
				m_winning_tiles[2] = __builtin_ctzll(b) + 2;
				m_winning_tiles[3] = __builtin_ctzll(b) + 3;
			}
			
			// horizontal check
			b = board & (board >> 7);
			b = (b >> 14) & b;
			if (b) {
				m_winning_tiles[0] = __builtin_ctzll(b);
				m_winning_tiles[1] = __builtin_ctzll(b) + 7;
				m_winning_tiles[2] = __builtin_ctzll(b) + 14;
				m_winning_tiles[3] = __builtin_ctzll(b) + 21;
			}
			
			// right diagonal
			b = board & (board >> 8);
			b = (b >> 16) & b;
			if (b) {
				m_winning_tiles[0] = __builtin_ctzll(b);
				m_winning_tiles[1] = __builtin_ctzll(b) + 8;
				m_winning_tiles[2] = __builtin_ctzll(b) + 16;
				m_winning_tiles[3] = __builtin_ctzll(b) + 24;
			}
			
			// left diagonal
			b = board & (board >> 6);
			b = (b >> 12) & b;
			if (b) {
				m_winning_tiles[0] = __builtin_ctzll(b);
				m_winning_tiles[1] = __builtin_ctzll(b) + 6;
				m_winning_tiles[2] = __builtin_ctzll(b) + 12;
				m_winning_tiles[3] = __builtin_ctzll(b) + 18;
			}
		}
	}
	
	return true;
}

int Game::m_which_hover() const {
	double mouse_x = (gp::get_mouse_x() / gp::get_framebuffer_width()) * 2.0f - 1.0f;
	mouse_x = (mouse_x - m_board_rect->left()) / (m_board_rect->right() - m_board_rect->left());
	if (fabs(1.0f - m_board_rect->bot()) < fabs(1.0f - m_board_rect->right())) {
		mouse_x += 0.5f;
		mouse_x *= 0.5f;
	}
	int hover = 0;
	for (int i = 0; i < 7; i++) {
		static const float holes_x[7] = { 0.14, 0.26, 0.38, 0.5, 0.62, 0.74, 0.86 };
		if (i == 6) {
			hover = 6;
			break;
		}
		if ((float) mouse_x < (holes_x[i + 1] + holes_x[i]) / 2.0f) {
			hover = i;
			break;
		}
	}
	return hover;
}

void Game::m_mouse_press(int button, double x, double y) {
	// do not do anything if the game is not in focus at the moment
	if (APP_MODE != APP_MODE_GAME) {
		return;
	}
	
	// only make the move if it is a human players turn
	if (m_board.get_turn() && m_is_one_player) {
		return;
	}
	
	int col = m_which_hover();
	
	// only make the move if it is legal
	if (!m_board.is_legal(col)) {
		return;
	}
	
	m_board.make_move(col);
}
