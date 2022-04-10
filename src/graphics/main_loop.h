// main loop
// will dispatch a single function call every frame

#pragma once

#include <functional>

namespace gp {
	
	class Main_loop {
		
	public:
		
		// static class
		Main_loop() = delete;
		Main_loop(const Main_loop&) = delete;
		Main_loop operator = (const Main_loop&) = delete;
		
		// main loop, must be called between gp::init() and gp::terminate()
		// must be called by the thread that called gp::init()
		// can be relaunched
		// terminates if glfwWindowShouldClose returns non-zero or if frame_callback returns false
		static void launch(const std::function <bool ()>& frame_callback);
		
		// change the function called by the main loop every frame
		static inline void update_frame_callback(const std::function <bool ()>& frame_callback) {
			m_frame_callback = frame_callback;
		}
		
	private:
		
		// the external function called every frame
		// return false to terminate, return true to continue
		static inline std::function <bool ()> m_frame_callback = nullptr;
		
	};
	
} /// namespace gp




