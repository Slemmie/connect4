// implements graphics.h

#include "graphics.h"

#include <iostream>
#include <thread>
#include <shared_mutex>

namespace gp {
	
	// variables local to this translation unit
	GLFWwindow* window_handle = nullptr;
	int window_width = 1280, window_height = 760;
	int framebuffer_width, framebuffer_height;
	
	// information about the initialization state of the gp namespace
	bool is_initialized = false;
	bool is_terminated = false;
	std::thread::id thread_id_of_init_caller;
	
	// mutex for keeping variables/library calls/etc. safe
	// will be acquired as a shared lock in some places
	std::shared_mutex mutex;
	
	// only ever called by gp::init()
	// is local to the current translation unit
	void set_glfw_callbacks();
	
	void init() {
		// be safe in case gp::init() and/or gp::terminate() somehow gets called simultaneously/more than once
		std::unique_lock lock(mutex);
		
		// make sure the gp::init() call is legal
		// must not already be initialized + must not be terminated previously
		if (is_initialized || is_terminated) {
			std::cerr << "[warning]: illegal call to gp::init()" << std::endl;
			return;
		}
		is_initialized = true;
		
		// keep the thread ID, log a warning in gp::terminate() if it gets called by a different thread
		thread_id_of_init_caller = std::this_thread::get_id();
		
		// initialize glfw
		if (!glfwInit()) {
			std::cerr << "[fatal]: failed to initialize glfw" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		// set glfw error callback before anything else
		glfwSetErrorCallback([] (int error_id, const char* error_message) -> void {
			std::cerr << "[glfw error (#" << error_id << ")]: " << error_message << std::endl;
		});
		
		// prepare opengl version
		// use version '4.6 core'
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		
		// create window
		window_handle = glfwCreateWindow(window_width, window_height, "connect4", nullptr, nullptr);
		if (!window_handle) {
			std::cerr << "[fatal]: failed to create window" << std::endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		// tie window context to current thread
		glfwMakeContextCurrent(window_handle);
		
		// initialize glfw
		if (glewInit() != GLEW_OK) {
			std::cerr << "[fatal]: failed to initialize glew" << std::endl;
			glfwDestroyWindow(window_handle);
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		// set gl viewport to match the framebuffer dimensions
		glfwGetFramebufferSize(window_handle, &framebuffer_width, &framebuffer_height);
		glViewport(0, 0, framebuffer_width, framebuffer_height);
		
		// enable vertical sync by default
		glfwSwapInterval(1);
		
		set_glfw_callbacks();
	}
	
	void set_glfw_callbacks() {
		// framebuffer resized
		glfwSetFramebufferSizeCallback(window_handle,
		[] (GLFWwindow* w_handle, int width, int height) -> void {
			// keep modified variables safe
			std::unique_lock lock(mutex);
			
			// resize gl viewport and update variables
			glViewport(0, 0, framebuffer_width = width, framebuffer_height = height);
			glfwGetWindowSize(w_handle, &window_width, &window_height);
		});
	}
	
	void terminate() {
		// be safe in case gp::init() and/or gp::terminate() somehow gets called simultaneously/more than once
		std::unique_lock lock(mutex);
		
		// make sure the gp::terminate() call is legal
		// must not be called without a previous call to gp::init() + must not re-terminate
		if (!is_initialized || is_terminated) {
			std::cerr << "[warning]: illegal call to gp::terminate()" << std::endl;
			return;
		}
		is_terminated = true;
		
		// log a warning if current thread does not match that of the gp::init() caller
		if (std::this_thread::get_id() != thread_id_of_init_caller) {
			std::cerr << "[warning]: gp::init()/gp::terminate() thread mismatch" << std::endl;
		}
		
		// destroy glfw window handle
		if (window_handle) {
			glfwDestroyWindow(window_handle);
		}
		
		// terminate glfw
		glfwTerminate();
	}
	
	// getters and setters
	
	GLFWwindow* get_window_handle() {
		return window_handle;
	}
	
	int get_framebuffer_width() {
		// keep the variables modified in glfw callbacks safe
		std::shared_lock lock(mutex);
		
		// make sure gp::init() has been called and gp::terminate() has not been called
		if (!is_initialized || is_terminated) {
			std::cerr << "[warning]: framebuffer width query ignored as " <<
			(is_terminated ?
			"graphics have been destructed" :
			"graphics have not been initialized") << std::endl;
			return 0;
		}
		
		return framebuffer_width;
	}
	
	int get_framebuffer_height() {
		// keep the variables modified in glfw callbacks safe
		std::shared_lock lock(mutex);
		
		// make sure gp::init() has been called and gp::terminate() has not been called
		if (!is_initialized || is_terminated) {
			std::cerr << "[warning]: framebuffer height query ignored as " <<
			(is_terminated ?
			"graphics have been destructed" :
			"graphics have not been initialized") << std::endl;
			return 0;
		}
		
		return framebuffer_height;
	}
	
	void set_window_size(const int width, const int height) {
		// keep the variables modified in glfw callbacks safe
		std::unique_lock lock(mutex);
		
		// make sure gp::init() has been called and gp::terminate() has not been called
		if (!is_initialized || is_terminated) {
			std::cerr << "[warning]: window resize request ignored as " <<
			(is_terminated ?
			"graphics have been destructed" :
			"graphics have not been initialized") << std::endl;
			return;
		}
		
		// glfwSetWindowSize(...) must only be called from the context bound thread
		if (std::this_thread::get_id() != thread_id_of_init_caller) {
			std::cerr <<
			"[warning]: window resize request ignored as it was called from an incompatible thread"
			<< std::endl;
			return;
		}
		
		// framebuffer resize callback will be launched and variables are updated there
		glfwSetWindowSize(window_handle, width, height);
	}
	
} /// namespace gp
