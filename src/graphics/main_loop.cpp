// implements main_loop.h

#include "main_loop.h"

#include "swap_window_buffers.h"

#include <glew/glew.h>
#include <glfw/glfw3.h>

namespace gp {
	
	extern GLFWwindow* window_handle;
	
	void Main_loop::launch(const std::function <bool ()>& frame_callback) {
		update_frame_callback(frame_callback);
		
		bool continue_loop = true;
		
		while (continue_loop && !glfwWindowShouldClose(window_handle)) {
			glClear(GL_COLOR_BUFFER_BIT);
			
			continue_loop &= m_frame_callback();
			
			glfwSwapBuffers(window_handle);
			
			glfwPollEvents();
		}
	}
	
} /// namespace gp
