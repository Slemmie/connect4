// implements menu.h

#include "menu.h"

#include "../graphics/graphics.h"

Main_menu::Main_menu() {
	m_shader = std::make_shared <gp::Shader> ();
	m_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/menu/menu_vertex.glsl");
	m_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/menu/menu_fragment.glsl");
	m_shader->assemble();
	
	m_rect = std::make_unique <gp::Rect> (m_shader, 1, 0, 1, 0);
	
	m_button_shader = std::make_shared <gp::Shader> ();
	m_button_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/menu/menu_button_vertex.glsl");
	m_button_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/menu/menu_button_fragment.glsl");
	m_button_shader->assemble();
	
	m_button_1player = std::make_shared <gp::Texture> ("src/img/player1_button.png");
	m_button_2player = std::make_shared <gp::Texture> ("src/img/player2_button.png");
	
	m_rect_1player = std::make_unique <gp::Rect> (m_button_shader, m_button_1player, 0.7, 0.57, 0.3, 0.7);
	m_rect_2player = std::make_unique <gp::Rect> (m_button_shader, m_button_2player, 0.43, 0.3, 0.3, 0.7);
}

bool Main_menu::frame() const {
	m_rect->render();
	
	m_rect_1player->render();
	m_rect_2player->render();
	
	return true;
}
