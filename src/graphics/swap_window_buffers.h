// wrapper for glfwSwapBuffers
// ensures the frame is not finished too fast when vsyn is enabled

#pragma once

struct GLFWwindow;

namespace gp {
	
	// only for use whenever vsyn is enabled/should be enabled
	void swap_window_buffers(GLFWwindow* window_handle);
	
} /// namespace gp
