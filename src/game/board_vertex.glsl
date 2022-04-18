// glsl vertex shader for 6x7 board

#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

uniform mat4 u_proj;
uniform mat4 u_model;

out vec2 frag_pos;
out vec2 local_pos;

void main() {
	gl_Position = u_proj * u_model * vec4(pos, 0.0f, 1.0f);
	
	frag_pos = (u_proj * u_model * vec4(pos, 0.0f, 1.0f)).xy;
	local_pos = tex;
}
