#pragma once

#include "Vector.hpp"
#include <cstdint>
class Material
{
  public:
	Material();

  private:
	Vector<4> _diffuse;
	uint16_t  _textureID;
};
