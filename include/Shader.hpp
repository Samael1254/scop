#pragma once

#include "Matrix.hpp"
#include "Vector.hpp"
#include <cstdint>
#include <glad/gl.h>
#include <string>

class Shader
{
  public:
	Shader(const std::string &vert, const std::string &frag);

	uint32_t id() const;

	void use() const;

	void setUniform(const std::string &name, bool value) const;
	void setUniform(const std::string &name, int value) const;
	void setUniform(const std::string &name, uint32_t value) const;
	void setUniform(const std::string &name, float value) const;
	void setUniform(const std::string &name, const Vector<2> &value) const;
	void setUniform(const std::string &name, const Vector<3> &value) const;
	void setUniform(const std::string &name, const Vector<4> &value) const;
	void setUniform(const std::string &name, const Matrix<2, 2> &value) const;
	void setUniform(const std::string &name, const Matrix<3, 3> &value) const;
	void setUniform(const std::string &name, const Matrix<4, 4> &value) const;
	void setUniform(const std::string &name, const Matrix<2, 3> &value) const;
	void setUniform(const std::string &name, const Matrix<3, 2> &value) const;
	void setUniform(const std::string &name, const Matrix<2, 4> &value) const;
	void setUniform(const std::string &name, const Matrix<4, 2> &value) const;
	void setUniform(const std::string &name, const Matrix<3, 4> &value) const;
	void setUniform(const std::string &name, const Matrix<4, 3> &value) const;

  private:
	uint32_t _id;

	static std::string _loadShader(const std::string &filename);
	static uint32_t    _compileShader(const std::string &filename, int type);
};
