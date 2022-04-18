// main game

#pragma once

#include "../application_mode.h"

#include "../graphics/shader.h"
#include "../graphics/rect.h"

#include <memory>

class Game {
	
public:
	
	Game(bool _is_one_player);
	
	bool frame() const;
	
private:
	
	std::shared_ptr <gp::Shader> m_background_shader;
	std::unique_ptr <gp::Rect> m_background_rect;
	
	std::shared_ptr <gp::Shader> m_board_shader;
	std::unique_ptr <gp::Rect> m_board_rect;
	
};
