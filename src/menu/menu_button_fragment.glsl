// glsl fragment shader for menu buttons

#version 460 core

uniform sampler2D u_texture;

uniform int u_is_hovering;

uniform float u_hover_time;

in vec2 frag_pos;
in vec2 frag_tex;
in vec2 local_pos;

out vec4 frag_color;

float dist(vec2 u, vec2 v) {
	return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}

void main() {
	frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	if (frag_pos.x < -1.0f || frag_pos.x > 1.0f || frag_pos.y < -1.0f || frag_pos.y > 1.0f) {
		frag_color.xyz = vec3(0.0f);
		return;
	}
	
	float x = frag_pos.x * 300.0f;
	float y = frag_pos.y * 300.0f;
	for (int i = -1; i < 1; i++) {
		for (int j = -1; j < 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			float hover_offset = u_hover_time * 5.0f;
			float xx = x + 9.0f * cos(x + y * 0.3f + hover_offset) * sin(x * 0.5f);
			float yy = y + 9.0f * sin(y + x * -0.8f + hover_offset) * cos(x * 0.5f);
			frag_color.xyz += vec3(0.2f * (1.0f / 50.0f) * dist(vec2(x, y), vec2(xx, yy)));
		}
	}
	frag_color.xyz += vec3(0.2f * pow((1.0f - dist(frag_tex, vec2(0.5f))), 2.0f));
	float edge_dist_x = min(local_pos.x, 1.0f - local_pos.x);
	float edge_dist_y = min(local_pos.y, 1.0f - local_pos.y);
	if (edge_dist_x < 0.02f || edge_dist_y < 0.08f) {
		frag_color.xyz *= min(edge_dist_x * 3.0f, edge_dist_y) * 30.0f;
	}
	frag_color.xyz *= 2.0f;
	
	if (u_is_hovering == 1) {
		frag_color.xyz *= 1.5f;
	}
	
	for (int i = 0; i < 3; i++) {
		frag_color[i] = min(frag_color[i], 1.0f);
	}
	
	// blend background color with texture color
	
	if (frag_tex.x < 0.0f || frag_tex.x > 1.0f || frag_tex.y < 0.0f || frag_tex.y > 1.0f) {
		return;
	}
	
	vec4 tex_color = texture(u_texture, frag_tex);
	
	float tex_weight = tex_color.w;
	float bag_weight = 1.0f - tex_weight;
	
	frag_color.xyz *= bag_weight;
	frag_color.xyz += tex_color.xyz * tex_weight;
}
