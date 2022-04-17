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
	
	extern double get_mouse_x();
	extern double get_mouse_y();
	
	extern double get_time();
	
	Rect::Rect(const std::shared_ptr <Shader>& _shader,
	double _top, double _bot, double _left, double _right) :
	m_shader(_shader),
	m_texture(nullptr),
	m_vao(0),
	m_vbo(0),
	m_ebo(0),
	m_top(0),
	m_bot(0),
	m_left(0),
	m_right(0),
	m_prev_time(0),
	m_hover_time(0),
	m_is_hovering(false)
	{
		// convert input coordinates from [0, 1] to [-1, 1]
		std::swap(_top, _bot);
		_top = _top * 2.0f - 1.0f;
		_bot = _bot * 2.0f - 1.0f;
		_left = _left * 2.0f - 1.0f;
		_right = _right * 2.0f - 1.0f;
		
		m_top = _top;
		m_bot = _bot;
		m_left = _left;
		m_right = _right;
		
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
	m_ebo(0),
	m_top(0),
	m_bot(0),
	m_left(0),
	m_right(0),
	m_prev_time(0),
	m_hover_time(0),
	m_is_hovering(false)
	{
		// convert input coordinates from [0, 1] to [-1, 1]
		std::swap(_top, _bot);
		_top = _top * 2.0f - 1.0f;
		_bot = _bot * 2.0f - 1.0f;
		_left = _left * 2.0f - 1.0f;
		_right = _right * 2.0f - 1.0f;
		
		m_top = _top;
		m_bot = _bot;
		m_left = _left;
		m_right = _right;
		
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
	
	void Rect::render() {
		// bind shader program and vertex array
		m_shader->bind();
		if (m_texture) {
			m_texture->bind();
		}
		glBindVertexArray(m_vao);
		
		// set a uniform for the current time since program start
		m_shader->set_uniform_1f("u_time", get_time());
		
		// set general position uniforms
		m_shader->set_uniform_1f("u_top", m_top);
		m_shader->set_uniform_1f("u_bot", m_bot);
		m_shader->set_uniform_1f("u_left", m_left);
		m_shader->set_uniform_1f("u_right", m_right);
		
		// set a flag telling whether the mouse is hovering the rectangle
		double mouse_x = (get_mouse_x() / framebuffer_width) * 2.0f - 1.0f;
		double mouse_y = (get_mouse_y() / framebuffer_height) * 2.0f - 1.0f;
		mouse_y = -mouse_y;
		m_is_hovering =
		mouse_y >= m_top && mouse_y <= m_bot &&
		mouse_x >= m_left && mouse_x <= m_right;
		m_shader->set_uniform_1i("u_is_hovering", m_is_hovering);
		// if the cursor is hovering...
		if (m_is_hovering) {
			m_hover_time += get_time() - m_prev_time;
		}
		m_shader->set_uniform_1f("u_hover_time", m_hover_time);
		
		// generate orthographic projection matrix
		glm::mat4 proj = glm::ortho(
		-float(framebuffer_width) * 0.5f, float(framebuffer_width) * 0.5f,
		-float(framebuffer_height) * 0.5f, float(framebuffer_height) * 0.5f,
		-1.0f, 1.0f);
		// set projection uniform
		m_shader->set_uniform_mat4("u_proj", proj);
		
		// scale
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(float(framebuffer_width) * 0.5f,
		float(framebuffer_height) * 0.5f, 1.0f));
		m_shader->set_uniform_mat4("u_model", model);
		
		// draw call
		glDrawElements(GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, (void*) 0);
		
		// unbind shader program and vertex array
		glBindVertexArray(0);
		if (m_texture) {
			m_texture->unbind();
		}
		m_shader->unbind();
		
		m_prev_time = get_time();
	}
	
} /// namespace gp
