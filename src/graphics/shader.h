// shader program class

#pragma once

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace gp {
	
	// enum for types of shaders
	enum Shader_type {
		
		SHADER_TYPE_NONE     = 0,
		SHADER_TYPE_VERTEX   = 1,
		SHADER_TYPE_FRAGMENT = 2
		
	};
	
	class Shader {
		
	public:
		
		Shader();
		
		~Shader();
		
		// bind the program for use
		void bind() const;
		// unbind the program
		void unbind() const;
		
		// get the program id
		constexpr unsigned int pid() const {
			return m_pid;
		}
		
		// add a glsl source before compiling
		void add_source(Shader_type shader_type, const std::string& source_path);
		
		// compile and link the added sources
		void assemble();
		
		// uniform setters
		#include "shader_uniform_func_decl.hpp"
		
	private:
		
		// program id
		unsigned int m_pid;
		
		// buffers for all added shader sources
		std::vector <std::pair <unsigned int, std::string>> m_sources;
		
		// keep track of whether the program has been assembled
		bool m_assembled;
		
	private:
		
		// table of uniform locations - such that only one external probe is necessary per uniform
		std::unordered_map <std::string, int> m_uniform_location_table;
		
		// get uniform location by querying the location table
		// or externally probing the result and populating the location table if needed
		int m_get_uniform_location(const std::string& uniform_name);
		
		// compile a single shader source and return the id
		unsigned int m_compile_source(unsigned int shader_type, const std::string& source) const;
		
		// utility for reading a shader source into a buffer
		std::string m_read_source(const std::string& source_path) const;
		
		// utility for error logging
		std::string m_shader_type_string(const unsigned int shader_type) const;
		std::string m_shader_type_string(const Shader_type shader_type) const;
		
	};
	
} /// namespace gp
