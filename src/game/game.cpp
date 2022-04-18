// implements game.h

#include "game.h"

Game::Game(bool _is_one_player) {
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
}

bool Game::frame() const {
	m_background_rect->render();
	
	// set the current board as a uniform variable
	// (temporary variables at the moment)
	m_board_shader->bind();
	unsigned int a = 1 | (1 << 7) | (1 << 14) | (1 << 21) | (1 << 10);
	unsigned int b =     (1 << 0) | (1 <<  7) | (1 << 14);
	m_board_shader->set_uniform_1ui("u_col0123", a);
	m_board_shader->set_uniform_1ui("u_col456", b);
	m_board_shader->unbind();
	m_board_rect->render();
	
	return true;
}
