#pragma once

#include "Vector.hpp"

class Light
{
  public:
	Light();
	Light(const Vector<3> &color, float brightness = 1.0F);

	Vector<3> getColor() const;
	float     getBrightness() const;

  protected:
	Vector<3> _color;
	float     _brightness;
};
