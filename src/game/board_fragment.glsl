// glsl fragment shader for 6x7 board

#version 460 core

const float holes_x[7] = {
	0.14, 0.26, 0.38, 0.5, 0.62, 0.74, 0.86,
};

const float holes_y[6] = {
	0.10,
	0.22 + 0.02,
	0.34 + 0.04,
	0.46 + 0.06,
	0.58 + 0.08,
	0.70 + 0.10
};

uniform float u_top;
uniform float u_bot;
uniform float u_left;
uniform float u_right;

uniform uint u_col0123;
uniform uint u_col456;

in vec2 frag_pos;
in vec2 local_pos;

out vec4 frag_color;

float map(float value, float min1, float max1, float min2, float max2) {
	return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

float dist(vec2 u, vec2 v) {
	return sqrt((u.x - v.x) * (u.x - v.x) + (u.y - v.y) * (u.y - v.y));
}

vec2 dist_to_edge() {
	float x = frag_pos.x;
	float y = frag_pos.y;
	bool sw = false;
	if (abs(1.0f - u_bot) < abs(1.0f - u_right)) {
		x /= 2.0f;
	} else {
		y /= 2.0f;
		sw = true;
	}
	vec2 res = vec2(
	min(abs(x - u_left), abs(x - u_right)),
	min(abs(y - u_top), abs(y - u_bot))
	);
	if (sw) {
		res = res.yx;
	}
	return res;
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
			float xx = x + 9.0f * cos(x + y * 0.3f) * sin(x * 0.5f);
			float yy = y + 9.0f * sin(y + x * -0.8f) * cos(x * 0.5f);
			frag_color.xyz += vec3(0.2f * (1.0f / 50.0f) * dist(vec2(x, y), vec2(xx, yy)));
		}
	}
	frag_color.xyz += vec3(0.2f * pow((1.0f - dist(local_pos, vec2(0.5f))), 2.0f));
	float edge_dist_x = dist_to_edge().x;
	float edge_dist_y = dist_to_edge().y;
	if (edge_dist_x < 0.01f || edge_dist_y < 0.03f) {
		frag_color.xyz *= min(edge_dist_x * 3.0f, edge_dist_y) * 30.0f;
	}
	frag_color.xyz *= 2.2f;
	
	for (int i = 0; i < 3; i++) {
		frag_color[i] = min(frag_color[i], 1.0f);
	}
	
	float mindist = 100.0f;
	vec2 hpos = vec2(map(frag_pos.x, u_left, u_right, -0.0f, 1.0f), map(frag_pos.y, u_top, u_bot, -0.0f, 1.0f));
	vec4 col = vec4(0.0f);
	if (abs(1.0f - u_bot) < abs(1.0f - u_right)) {
		hpos.x += 0.5f;
		hpos.x *= 0.5f;
	} else {
		hpos.y += 0.5f;
		hpos.y *= 0.5f;
	}
	for (int i = 0; i < 6 * 7; i++) {
		float d = dist(vec2(holes_x[i % 7], holes_y[i / 7]), hpos);
		if (d < mindist) {
			mindist = d;
			int c = i % 7;
			uint cmask = ((c < 4 ? (u_col0123 >> (c * 7)) : (u_col456 >> ((c - 4) * 7)))) & ((1 << 7) - 1);
			int r = i / 7;
			for (int j = 0; j < r; j++) {
				cmask >>= 1;
			}
			if (cmask > 1) {
				if ((cmask & 1) != 0) {
					col = vec4(0.8f, 0.1f, 0.2f, 0.9f);
				} else {
					col = vec4(0.2f, 0.1f, 0.8f, 0.9f);
				}
			} else {
				col = vec4(0.0f);
			}
		}
	}
	
	if (mindist < 0.05f) {
		frag_color.xyz *= 0.1f;
		frag_color.xyz += col.xyz * col.w;
	} else if (mindist < 0.055f) {
		frag_color.xyz = (vec3(1.0f) - frag_color.xyz) * 0.4f;
	}
}
