// header for graphics initialization/termination
// contains utility, window handle getter, window dimension getters, callback setters, etc.

#pragma once

#include "main_loop.h"

#include <glew/glew.h>
#include <glfw/glfw3.h>

namespace gp {
	
	// must be called exactly once
	// should be called by the thread expecting to communicate with the gp namespace
	void init();
	
	// must be called exactly once
	// should be called by the same thread that called gp::init()
	void terminate();
	
	// getters
	GLFWwindow* get_window_handle();
	int get_framebuffer_width();
	int get_framebuffer_height();
	
	// setters
	// must be called by the thread that called gp::init()
	void set_window_size(const int width, const int height);
	
} /// namespace gp
