// header for graphics initialization/termination
// contains utility, window handle getter, window dimension getters, callback setters, etc.

#pragma once

#include "main_loop.h"

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include <functional>

namespace gp {
	
	// must be called exactly once
	// should be called by the thread expecting to communicate with the gp namespace
	void init();
	
	// can be called from anywhere, from any thread
	// push a function to a list of functions called right before terminating
	void on_terminate(const std::function <void ()>& func);
	
	// must be called exactly once
	// should be called by the same thread that called gp::init()
	void terminate();
	
	// callbacks
	// a variable number of functions can be bound to a single event at any time
	// push_callback_mouse_press -> button, x, y
	void push_callback_mouse_press(const std::function <void (int, double, double)>& func);
	
	// getters
	GLFWwindow* get_window_handle();
	int get_framebuffer_width();
	int get_framebuffer_height();
	double get_mouse_x();
	double get_mouse_y();
	double get_time();
	
	// setters
	// must be called by the thread that called gp::init()
	void set_window_size(const int width, const int height);
	
} /// namespace gp
