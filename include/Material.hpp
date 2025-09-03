#pragma once

#include "Vector.hpp"
#include <cstdint>
class Material
{
  public:
	Material();

  private:
	Vector<4> _diffuse;
	Vector<4> _ambient;
	int16_t   _textureID;
};
