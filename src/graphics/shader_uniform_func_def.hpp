// collection of shader uniform setter function definitions

void Shader::set_uniform_1f(const char* name, float_t v0) {
	glUniform1f(m_get_uniform_location(name), v0);
}

void Shader::set_uniform_2f(const char* name, float_t v0, float_t v1) {
	glUniform2f(m_get_uniform_location(name), v0, v1);
}

void Shader::set_uniform_3f(const char* name, float_t v0, float_t v1, float_t v2) {
	glUniform3f(m_get_uniform_location(name), v0, v1, v2);
}

void Shader::set_uniform_4f(const char* name, float_t v0, float_t v1, float_t v2, float_t v3) {
	glUniform4f(m_get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1i(const char* name, int32_t v0) {
	glUniform1i(m_get_uniform_location(name), v0);
}

void Shader::set_uniform_2i(const char* name, int32_t v0, int32_t v1) {
	glUniform2i(m_get_uniform_location(name), v0, v1);
}

void Shader::set_uniform_3i(const char* name, int32_t v0, int32_t v1, int32_t v2) {
	glUniform3i(m_get_uniform_location(name), v0, v1, v2);
}

void Shader::set_uniform_4i(const char* name, int32_t v0, int32_t v1, int32_t v2, int32_t v3) {
	glUniform4i(m_get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1ui(const char* name, uint32_t v0) {
	glUniform1ui(m_get_uniform_location(name), v0);
}

void Shader::set_uniform_2ui(const char* name, uint32_t v0, uint32_t v1) {
	glUniform2ui(m_get_uniform_location(name), v0, v1);
}

void Shader::set_uniform_3ui(const char* name, uint32_t v0, uint32_t v1, uint32_t v2) {
	glUniform3ui(m_get_uniform_location(name), v0, v1, v2);
}

void Shader::set_uniform_4ui(const char* name, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3) {
	glUniform4ui(m_get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_1fv(const char* name, uint32_t count, const float_t* v0) {
	glUniform1fv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec2f(const char* name, const glm::vec2& v0) {
	glUniform2fv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_2fv(const char* name, uint32_t count, const float_t* v0) {
	glUniform2fv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec3f(const char* name, const glm::vec3& v0) {
	glUniform3fv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_3fv(const char* name, uint32_t count, const float_t* v0) {
	glUniform3fv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec4f(const char* name, const glm::vec4& v0) {
	glUniform4fv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_4fv(const char* name, uint32_t count, const float_t* v0) {
	glUniform4fv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_1iv(const char* name, uint32_t count, const int32_t* v0) {
	glUniform1iv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec2i(const char* name, const glm::ivec2& v0) {
	glUniform2iv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_2iv(const char* name, uint32_t count, const int32_t* v0) {
	glUniform2iv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec3i(const char* name, const glm::ivec3& v0) {
	glUniform3iv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_3iv(const char* name, uint32_t count, const int32_t* v0) {
	glUniform3iv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec4i(const char* name, const glm::ivec4& v0) {
	glUniform4iv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_4iv(const char* name, uint32_t count, const int32_t* v0) {
	glUniform4iv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_1uiv(const char* name, uint32_t count, const uint32_t* v0) {
	glUniform1uiv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec2ui(const char* name, const glm::uvec2& v0) {
	glUniform2uiv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_2uiv(const char* name, uint32_t count, const uint32_t* v0) {
	glUniform2uiv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec3ui(const char* name, const glm::uvec3& v0) {
	glUniform3uiv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_3uiv(const char* name, uint32_t count, const uint32_t* v0) {
	glUniform3uiv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_vec4ui(const char* name, const glm::uvec4& v0) {
	glUniform4uiv(m_get_uniform_location(name), 1, glm::value_ptr(v0));
}

void Shader::set_uniform_4uiv(const char* name, uint32_t count, const uint32_t* v0) {
	glUniform4uiv(m_get_uniform_location(name), count, v0);
}

void Shader::set_uniform_mat2(const char* name, const glm::mat2& v0, bool transpose) {
	glUniformMatrix2fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat2fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix2fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat3(const char* name, const glm::mat3& v0, bool transpose) {
	glUniformMatrix3fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat3fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix3fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat4(const char* name, const glm::mat4& v0, bool transpose) {
	glUniformMatrix4fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat4fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix4fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat2x3(const char* name, const glm::mat2x3& v0, bool transpose) {
	glUniformMatrix2x3fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat2x3fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix2x3fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat3x2(const char* name, const glm::mat3x2& v0, bool transpose) {
	glUniformMatrix3x2fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat3x2fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix3x2fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat2x4(const char* name, const glm::mat2x4& v0, bool transpose) {
	glUniformMatrix2x4fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat2x4fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix2x4fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat4x2(const char* name, const glm::mat4x2& v0, bool transpose) {
	glUniformMatrix4x2fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat4x2fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix4x2fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat3x4(const char* name, const glm::mat3x4& v0, bool transpose) {
	glUniformMatrix3x4fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat3x4fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix3x4fv(m_get_uniform_location(name), count, transpose, v0);
}

void Shader::set_uniform_mat4x3(const char* name, const glm::mat4x3& v0, bool transpose) {
	glUniformMatrix4x3fv(m_get_uniform_location(name), 1, transpose, glm::value_ptr(v0));
}

void Shader::set_uniform_mat4x3fv(const char* name, uint32_t count, float_t* v0, bool transpose) {
	glUniformMatrix4x3fv(m_get_uniform_location(name), count, transpose, v0);
}
