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
	
};
