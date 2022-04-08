// header for graphics initialization/termination
// contains utility, window handle getter, window dimension getters, callback setters, etc.

#pragma once

#include <glew/glew.h>
#include <glfw/glfw3.h>

namespace gp {
	
	// must be called exactly once
	// should be called by the thread expecting to communicate with the gp namespace
	void init();
	
	// must be called exactly once
	// should be called by the same thread that called gp::init()
	void terminate();
	
} /// namespace gp
