// rectangle renderer with given shader program and dimensions

#pragma once

#include "shader.h"
#include "texture.h"

#include <memory>

namespace gp {
	
	class Rect {
		
	public:
		
		// construct rectangle without a texture
		// the shader will not get any texture coordinate information
		Rect(const std::shared_ptr <Shader>& _shader,
		double _top, double _bot, double _left, double _right);
		
		// construct rectangle with a texture
		// the shader gets texture coordinates
		// the gp::Rect will bind and unbind the given texture correctly
		Rect(const std::shared_ptr <Shader>& _shader, const std::shared_ptr <Texture>& _texture,
		double _top, double _bot, double _left, double _right);
		
		~Rect();
		
		void render() const;
		
	private:
		
		std::shared_ptr <Shader> m_shader;
		
		std::shared_ptr <Texture> m_texture;
		
		// vertex array id, buffers ids
		unsigned int m_vao, m_vbo, m_ebo;
		
	};
	
} /// namespace gp
