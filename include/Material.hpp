#pragma once

#include "Vector.hpp"
#include <cstdint>
class Material
{
  public:
	Material();

  private:
	Vector<4> _diffuse;
	int16_t   _textureID;
};
