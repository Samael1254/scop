#pragma once

#include "Vector.hpp"
#include <cstdint>
class Material
{
  public:
	Material();

	const Vector<3> &getDiffuse() const;
	const Vector<3> &getAmbient() const;

  private:
	Vector<3> _diffuse;
	Vector<3> _ambient;
	int16_t   _textureID;
};
