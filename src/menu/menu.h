// main menu entry point

#pragma once

#include "../application_mode.h"

#include "../graphics/shader.h"
#include "../graphics/rect.h"

#include <memory>

class Main_menu {
	
public:
	
	Main_menu();
	
	bool frame() const;
	
private:
	
	std::shared_ptr <gp::Shader> m_shader;
	std::unique_ptr <gp::Rect> m_rect;
	
	std::shared_ptr <gp::Shader> m_button_shader;
	std::shared_ptr <gp::Texture> m_button_1player;
	std::shared_ptr <gp::Texture> m_button_2player;
	std::unique_ptr <gp::Rect> m_rect_1player;
	std::unique_ptr <gp::Rect> m_rect_2player;
	
};
