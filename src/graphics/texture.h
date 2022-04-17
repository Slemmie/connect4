// standardized texture class

#pragma once

#include <string>

namespace gp {
	
	class Texture {
		
	public:
		
		Texture(const std::string& _filepath);
		
		~Texture();
		
		void bind() const;
		
		void unbind() const;
		
	private:
		
		unsigned int m_id;
		
		uint8_t* m_texture_buffer;
		
		int m_width;
		int m_height;
		int m_bpp;
		
	};
	
} /// namespace gp
