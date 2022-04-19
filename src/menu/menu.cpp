// implements menu.h

#include "menu.h"

#include "../graphics/graphics.h"

Main_menu::Main_menu() :
m_game(nullptr)
{
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
	
	// set mouse press callback
	gp::push_callback_mouse_press([this] (int button, double x, double y) ->
	void { this->check_if_pressed(button, x, y); });
}

bool Main_menu::frame() {
	m_rect->render();
	
	m_rect_1player->render();
	m_rect_2player->render();
	
	return true;
}

extern Application_mode APP_MODE;

void Main_menu::check_if_pressed(int button, double x, double y) {
	// do not do anything if the menu is not in focus at the moment
	if (APP_MODE != APP_MODE_MENU) {
		return;
	}
	
	// only consider the button press if it was the left button
	if (button != 1) {
		return;
	}
	
	double mouse_x = x / gp::get_framebuffer_width();
	double mouse_y = y / gp::get_framebuffer_height();
	mouse_y = 1.0f - mouse_y;
	if (mouse_x >= 0.3 && mouse_x <= 0.7 && mouse_y >= 0.57 && mouse_y <= 0.7) {
		APP_MODE = APP_MODE_GAME;
		m_game = std::make_unique <Game> (true);
		gp::Main_loop::launch([this] () -> bool { return m_game->frame(); });
	} else if (mouse_x >= 0.3 && mouse_x <= 0.7 && mouse_y >= 0.3 && mouse_y <= 0.43) {
		APP_MODE = APP_MODE_GAME;
		m_game = std::make_unique <Game> (false);
		gp::Main_loop::launch([this] () -> bool { return m_game->frame(); });
	}
}
