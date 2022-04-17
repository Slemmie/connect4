// implements texture.h

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <glew/glew.h>

#include <iostream>

namespace gp {
	
	Texture::Texture(const std::string& _filepath) :
	m_id(~0u),
	m_texture_buffer(nullptr),
	m_width(0),
	m_height(0),
	m_bpp(0)
	{
		stbi_set_flip_vertically_on_load(1);
		
		m_texture_buffer = stbi_load(_filepath.c_str(), &m_width, &m_height, &m_bpp, 4);
		// log a warning if stbi_load failed
		if (!m_texture_buffer) {
			std::cerr << "[warning]: stbi failed to load '" << _filepath << "'" << std::endl;
			m_id = ~0;
			return;
		}
		
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_texture_buffer);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		if (m_texture_buffer) {
			stbi_image_free(m_texture_buffer);
		}
	}
	
	Texture::~Texture() {
		// do not delete the texture if it was never generated
		if (m_id != ~0u) {
			glDeleteTextures(1, &m_id);
		}
	}
	
	void Texture::bind() const {
		// do not allow binding if the texture id was never generated
		if (m_id == ~0u) {
			std::cerr << "[warning]: ignoring attempt to bind a texture with bad id" << std::endl;
			return;
		}
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
	
	void Texture::unbind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
} /// namespace gp
