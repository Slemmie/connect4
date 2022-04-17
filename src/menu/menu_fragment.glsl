// glsl fragment shader for menu background

#version 460 core

in vec2 frag_pos;

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
			float xx = x + 2.0f * sin(x + y * 0.3f);
			float yy = y + 2.0f * cos(y + x * -0.3f);
			frag_color.xyz += vec3(0.2f * (1.0f / 50.0f) * dist(vec2(x, y), vec2(xx, yy)));
		}
	}
	frag_color.xyz += vec3(0.1f * (1.0f - dist(frag_pos, vec2(0.0f))));
}
