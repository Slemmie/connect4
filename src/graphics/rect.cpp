// implements rect.h

#include "rect.h"

#include "shader.h"

#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <utility>

namespace gp {
	
	extern int framebuffer_width;
	extern int framebuffer_height;
	
	Rect::Rect(const std::shared_ptr <Shader>& _shader,
	double _top, double _bot, double _left, double _right) :
	m_shader(_shader),
	m_texture(nullptr),
	m_vao(0),
	m_vbo(0),
	m_ebo(0)
	{
		// convert input coordinates from [0, 1] to [-1, 1]
		_top = _top * 2.0f - 1.0f;
		_bot = _bot * 2.0f - 1.0f;
		_left = _left * 2.0f - 1.0f;
		_right = _right * 2.0f - 1.0f;
		
		// set coordinate buffer values
		float_t vertices[2 * 4] = {
			(float_t) _left, (float_t) _top,
			(float_t) _right, (float_t) _top,
			(float_t) _left, (float_t) _bot,
			(float_t) _right, (float_t) _bot
		};
		uint32_t indices[3 * 2] = {
			0, 1, 2, // tl >>> tr >>> bl
			1, 3, 2  // tr >>> br >>> bl
		};
		
		// generate buffers, vertex array, set vertex pointers, set buffer data, etc.
		_shader->bind();
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);
		glBindVertexArray(m_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float_t) * 2 * 4, vertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3 * 2, indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 2, (void*) 0);
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		_shader->unbind();
	}
	
	Rect::Rect(const std::shared_ptr <Shader>& _shader, const std::shared_ptr <Texture>& _texture,
	double _top, double _bot, double _left, double _right) :
	m_shader(_shader),
	m_texture(_texture),
	m_vao(0),
	m_vbo(0),
	m_ebo(0)
	{
		// convert input coordinates from [0, 1] to [-1, 1]
		_top = _top * 2.0f - 1.0f;
		_bot = _bot * 2.0f - 1.0f;
		_left = _left * 2.0f - 1.0f;
		_right = _right * 2.0f - 1.0f;
		
		// set coordinate buffer values + texture coordinates
		float_t vertices[4 * 4] = {
			(float_t) _left,  (float_t) _top, 0.0f, 0.0f,
			(float_t) _right, (float_t) _top, 1.0f, 0.0f,
			(float_t) _left,  (float_t) _bot, 0.0f, 1.0f,
			(float_t) _right, (float_t) _bot, 1.0f, 1.0f
		};
		uint32_t indices[3 * 2] = {
			0, 1, 2, // tl >>> tr >>> bl
			1, 3, 2  // tr >>> br >>> bl
		};
		
		// generate buffers, vertex array, set vertex pointers, set buffer data, etc.
		_shader->bind();
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);
		glBindVertexArray(m_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float_t) * 4 * 4, vertices, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 3 * 2, indices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 4, (void*) 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float_t) * 4, (void*) (sizeof(float_t) * 2));
		glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		m_shader->set_uniform_1i("u_texture", (int32_t) 0);
		
		_shader->unbind();
	}
	
	Rect::~Rect() {
		glDeleteVertexArrays(1, &m_vao);
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ebo);
	}
	
	void Rect::render() const {
		// bind shader program and vertex array
		m_shader->bind();
		if (m_texture) {
			m_texture->bind();
		}
		glBindVertexArray(m_vao);
		
		// generate orthographic projection matrix
		glm::mat4 proj = glm::ortho(
		-float(framebuffer_width) * 0.5f, float(framebuffer_width) * 0.5f,
		-float(framebuffer_height) * 0.5f, float(framebuffer_height) * 0.5f,
		-1.0f, 1.0f);
		// set projection uniform
		m_shader->set_uniform_mat4("u_proj", proj);
		
		// scale
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(float(framebuffer_width), float(framebuffer_height), 1.0f));
		m_shader->set_uniform_mat4("u_model", model);
		
		// draw call
		glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*) 0);
		
		// unbind shader program and vertex array
		glBindVertexArray(0);
		if (m_texture) {
			m_texture->unbind();
		}
		m_shader->unbind();
	}
	
} /// namespace gp
