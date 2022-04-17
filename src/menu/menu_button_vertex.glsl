// glsl vertex shader for menu buttons

#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

uniform mat4 u_proj;
uniform mat4 u_model;

out vec2 frag_pos;
out vec2 frag_tex;
out vec2 local_pos;
out int is_hovering;

void main() {
	gl_Position = u_proj * u_model * vec4(pos, 0.0f, 1.0f);
	
	frag_pos = pos;
	frag_tex = tex;
	local_pos = tex;
	
	// adjust texture coordinates such that the margin to the edge increases
	if (frag_tex.x < 0.5f) {
		frag_tex.x -= 0.05f;
	} else {
		frag_tex.x += 0.05f;
	}
	if (frag_tex.y < 0.5f) {
		frag_tex.y -= 0.05f;
	} else {
		frag_tex.y += 0.05f;
	}
}
