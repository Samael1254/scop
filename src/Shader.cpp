#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

Shader::Shader(const std::string &vert, const std::string &frag)
{
	uint32_t vertexShader = _compileShader(vert, GL_VERTEX_SHADER);
	uint32_t fragmentShader = _compileShader(frag, GL_FRAGMENT_SHADER);

	uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		throw std::runtime_error("failed to link shaders: " + std::string(infoLog));
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	_id = shaderProgram;
}

uint32_t Shader::id() const
{
	return _id;
}

void Shader::use() const
{
	glUseProgram(_id);
}

// Set Uniform Values

void Shader::setUniform(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), static_cast<int>(value));
}

void Shader::setUniform(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, uint32_t value) const
{
	glUniform1ui(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, const Vector<2> &value) const
{
	glUniform2f(glGetUniformLocation(_id, name.c_str()), value[0], value[1]);
}

void Shader::setUniform(const std::string &name, const Vector<3> &value) const
{
	glUniform3f(glGetUniformLocation(_id, name.c_str()), value[0], value[1], value[2]);
}

void Shader::setUniform(const std::string &name, const Vector<4> &value) const
{
	glUniform4f(glGetUniformLocation(_id, name.c_str()), value[0], value[1], value[2], value[3]);
}

void Shader::setUniform(const std::string &name, const Matrix<2, 2> &value) const
{
	float array[4];
	value.arrayCMO(array);
	glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<3, 3> &value) const
{
	float array[9];
	value.arrayCMO(array);
	glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<4, 4> &value) const
{
	float array[16];
	value.arrayCMO(array);
	glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<2, 3> &value) const
{
	float array[6];
	value.arrayCMO(array);
	glUniformMatrix3x2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<3, 2> &value) const
{
	float array[6];
	value.arrayCMO(array);
	glUniformMatrix2x3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<2, 4> &value) const
{
	float array[8];
	value.arrayCMO(array);
	glUniformMatrix4x2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<4, 2> &value) const
{
	float array[8];
	value.arrayCMO(array);
	glUniformMatrix2x4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<3, 4> &value) const
{
	float array[12];
	value.arrayCMO(array);
	glUniformMatrix4x3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

void Shader::setUniform(const std::string &name, const Matrix<4, 3> &value) const
{
	float array[12];
	value.arrayCMO(array);
	glUniformMatrix3x4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, array);
}

// Shader loading and compilation

uint32_t Shader::_compileShader(const std::string &filename, int type)
{
	uint32_t    shader = glCreateShader(type);
	std::string shaderStr = _loadShader(filename);
	const char *shaderSource = shaderStr.c_str();
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		throw std::runtime_error("failed to compile shader " + filename + ": " + infoLog);
	}
	return shader;
}

std::string Shader::_loadShader(const std::string &filename)
{
	std::ifstream is("shaders/" + filename);
	if (!is.is_open())
		throw std::runtime_error("failed to open shader file: " + filename);
	std::stringstream ss;
	ss << is.rdbuf();
	return ss.str();
}
