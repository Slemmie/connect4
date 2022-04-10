// implements swap_window_buffers.h

#include "swap_window_buffers.h"

#include <glfw/glfw3.h>

#include <thread>
#include <chrono>
#include <cmath>
#include <iostream>

namespace gp {
	
	typedef unsigned long long timestamp;
	
	void swap_window_buffers(GLFWwindow* window_handle) {
		static double prev_seconds;
		
		// special case on first call to gp::swap_window_buffers
		static bool is_first_call = true;
		if (is_first_call) {
			is_first_call = false;
			glfwSwapBuffers(window_handle);
			prev_seconds = glfwGetTime();
			return;
		}
		
		glfwSwapBuffers(window_handle);
		
		// check if vsync failed or if it failed previously
		// if so, manually put thread to sleep
		
		static bool v_sync_broken = false;
		
		if (!v_sync_broken) {
			timestamp diff_micros = static_cast <timestamp> ((glfwGetTime() - prev_seconds) * 1e6);
			
			// if the frame exceeds frame time for ~166 FPS
			if (diff_micros < timestamp(6000)) {
				v_sync_broken = true;
				std::cerr << "[warning]: vertical sync broke; enabling manual backup delay" << std::endl;
			}
		}
		
		if (v_sync_broken) {
			// target 60 FPS
			// add a tiny constant for breathing room with ending the frame
			// (the extra constants inside this function for instance)
			timestamp diff_micros = static_cast <timestamp> ((glfwGetTime() - prev_seconds) * 1e6);
			timestamp required_sleep_micros = static_cast <timestamp> (std::max(0LL,
			(long long) std::round(1e6 / 60.0 - 50.0) - (long long) diff_micros));
			
			std::this_thread::sleep_for(std::chrono::microseconds(required_sleep_micros));
		}
		
		prev_seconds = glfwGetTime();
	}
	
} /// namespace gp
