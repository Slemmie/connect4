// implements shader.h

#include "shader.h"

#include <glew/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

namespace gp {
	
	Shader::Shader() :
	m_pid(0),
	m_assembled(false)
	{ }
	
	Shader::~Shader() {
		if (m_pid) {
			glDeleteProgram(m_pid);
			m_pid = 0;
		}
	}
	
	void Shader::bind() const {
		// only use program if it exists
		if (!m_pid) {
			std::cerr << "[warning]: attempted to bind an unassembled shader program" << std::endl;
			return;
		}
		
		glUseProgram(m_pid);
	}
	
	void Shader::unbind() const {
		glUseProgram(0);
	}
	
	void Shader::add_source(Shader_type shader_type, const std::string& source_path) {
		// do not allow adding new sources if the program has already been assembled
		if (m_assembled) {
			std::cerr <<
			"[warning]: ignoring attempt to add shader source as the program has already been assembled"
			<< std::endl;
			return;
		}
		
		// only allow vertex/fragment shader types
		if (shader_type != SHADER_TYPE_VERTEX && shader_type != SHADER_TYPE_FRAGMENT) {
			std::cerr << "[warning]: ignoring attempt to add shader source as the shader type (" <<
			m_shader_type_string(shader_type) << ") is unsupported" << std::endl;
			return;
		}
		
		m_sources.emplace_back(shader_type == SHADER_TYPE_VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER,
		m_read_source(source_path));
		
		// if for whatever reason the buffer ends up empty, do not keep it
		if (m_sources.back().second.empty()) {
			m_sources.pop_back();
		}
	}
	
	void Shader::assemble() {
		// do not allow re-assembling
		if (m_assembled) {
			std::cerr <<
			"[warning]: ignoring attempt to re-assemble already assembled shader program"
			<< std::endl;
			return;
		}
		
		// get program id
		m_pid = glCreateProgram();
		
		// compile sources and accumulate source ids
		std::vector <unsigned int> source_ids;
		for (const auto& source : m_sources) {
			source_ids.push_back(m_compile_source(source.first, source.second));
			
			// if the acquired id is ~0u, do not use the shader, it failed to compile
			if (source_ids.back() == ~0u) {
				source_ids.pop_back();
			}
		}
		
		// warn if there are no sources loaded
		if (source_ids.empty()) {
			std::cerr << "[warning]: attempting to assemble shader program with no shader sources" << std::endl;
		}
		
		// attach source ids
		for (unsigned int id : source_ids) {
			glAttachShader(m_pid, id);
		}
		
		// link shader program
		glLinkProgram(m_pid);
		
		// final validation
		glValidateProgram(m_pid);
		
		// delete source ids
		for (unsigned int id : source_ids) {
			glDeleteShader(id);
		}
		
		// free up space
		m_sources.clear();
		
		// assembling done, set flag
		m_assembled = true;
	}
	
	int Shader::m_get_uniform_location(const std::string& uniform_name) {
		// do not allow querying for uniform location before the program exists
		if (!m_pid) {
			std::cerr << "[warning]: attempted to query a uniform location '" <<
			uniform_name << "' on an unassembled shader program" << std::endl;
			return 0;
		}
		
		// if the location table has a matching entry already, default to that
		auto it = m_uniform_location_table.find(uniform_name);
		if (it != m_uniform_location_table.end()) {
			return it->second;
		}
		
		// add an entry before returning the result
		return m_uniform_location_table[uniform_name] = glGetUniformLocation(m_pid, uniform_name.c_str());
	}
	
	unsigned int Shader::m_compile_source(unsigned int shader_type, const std::string& source) const {
		// obtain the id
		unsigned int id = glCreateShader(shader_type);
		
		// load buffer
		const char* cbuf = source.c_str();
		glShaderSource(id, 1, &cbuf, nullptr);
		
		// compile buffer
		glCompileShader(id);
		
		// check compilation result
		int compilation_result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &compilation_result);
		if (compilation_result == GL_FALSE) {
			// something went wrong, obtain error message
			int err_msg_len;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &err_msg_len);
			std::string err_msg(err_msg_len, '\0');
			glGetShaderInfoLog(id, err_msg_len, &err_msg_len, err_msg.data());
			std::cerr << "[warning]: " << m_shader_type_string(shader_type) <<
			" shader compilation failed: " << err_msg << std::endl;
			glDeleteShader(id);
			return ~0u;
		}
		
		return id;
	}
	
	std::string Shader::m_read_source(const std::string& source_path) const {
		std::ifstream inf(source_path);
		
		// return an empty buffer if the file is unavailable
		if (!inf.is_open()) {
			std::cerr << "[warning]: failed to open shader source file '" << source_path << "'" << std::endl;
			return "";
		}
		
		std::stringstream stream;
		stream << inf.rdbuf();
		
		inf.close();
		
		return stream.str();
	}
	
	std::string Shader::m_shader_type_string(const unsigned int shader_type) const {
		switch (shader_type) {
			case GL_VERTEX_SHADER          : return "vertex";
			case GL_TESS_CONTROL_SHADER    : return "tessellation control";
			case GL_TESS_EVALUATION_SHADER : return "tessellation evaluation";
			case GL_GEOMETRY_SHADER        : return "geometry";
			case GL_FRAGMENT_SHADER        : return "fragment";
			case GL_COMPUTE_SHADER         : return "compute";
			default: break;
		};
		return "[unknown shader type]";
	}
	
	std::string Shader::m_shader_type_string(const Shader_type shader_type) const {
		switch (shader_type) {
			case Shader_type::SHADER_TYPE_VERTEX   : return "vertex";
			case Shader_type::SHADER_TYPE_FRAGMENT : return "fragment";
			default: break;
		};
		return "[unknown shader type]";
	}
	
	// uniform setter implementations
	#include "shader_uniform_func_def.hpp"
	
} /// namespace gp
