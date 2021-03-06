// main game

#pragma once

#include "../application_mode.h"

#include "../graphics/shader.h"
#include "../graphics/rect.h"

#include "bit_board.h"

#include <memory>
#include <atomic>
#include <thread>

class Game {
	
public:
	
	Game(bool _is_one_player);
	
	bool frame();
	
private:
	
	std::shared_ptr <gp::Shader> m_background_shader;
	std::unique_ptr <gp::Rect> m_background_rect;
	
	std::shared_ptr <gp::Shader> m_board_shader;
	std::unique_ptr <gp::Rect> m_board_rect;
	
	bool m_is_one_player;
	
private:
	
	void m_mouse_press(int button, double x, double y);
	
	int m_which_hover() const;
	
	unsigned int m_winning_tiles[4];
	
private:
	
	Board m_board;
	
private:
	
	std::atomic <bool> m_is_AI_searching = false;
	std::atomic <bool> m_is_AI_done = false;
	Board m_board_AI_result;
	std::unique_ptr <std::thread> m_AI_thread = nullptr;
	
};
