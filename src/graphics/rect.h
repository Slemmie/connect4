// rectangle renderer with given shader program and dimensions

#pragma once

#include "shader.h"

#include <memory>

namespace gp {
	
	class Rect {
		
	public:
		
		Rect(const std::shared_ptr <Shader>& _shader,
		double _top, double _bot, double _left, double _right);
		
		~Rect();
		
		void render() const;
		
	private:
		
		std::shared_ptr <Shader> m_shader;
		
		// vertex array id, buffers ids
		unsigned int m_vao, m_vbo, m_ebo;
		
	};
	
} /// namespace gp
