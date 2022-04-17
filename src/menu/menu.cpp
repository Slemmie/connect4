// implements menu.h

#include "menu.h"

Main_menu::Main_menu() {
	m_shader = std::make_shared <gp::Shader> ();
	m_shader->add_source(gp::SHADER_TYPE_VERTEX, "src/menu/menu_vertex.glsl");
	m_shader->add_source(gp::SHADER_TYPE_FRAGMENT, "src/menu/menu_fragment.glsl");
	m_shader->assemble();
	
	m_rect = std::make_unique <gp::Rect> (m_shader, 0, 1, 0, 1);
}

bool Main_menu::frame() const {
	m_rect->render();
	
	return true;
}
