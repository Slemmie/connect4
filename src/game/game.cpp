// implements game.h

#include "game.h"

Game::Game(bool _is_one_player) {
	// use the menu background shader as our background as well
	m_background_shader = std::make_shared <gp::Shader> ();
	m_background_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/menu/menu_vertex.glsl");
	m_background_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/menu/menu_fragment.glsl");
	m_background_shader->assemble();
	
	m_background_rect = std::make_unique <gp::Rect> (m_background_shader, 1, 0, 1, 0);
}

bool Game::frame() const {
	m_background_rect->render();
	
	return true;
}
