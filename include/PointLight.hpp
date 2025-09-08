#pragma once

#include "Light.hpp"
#include "Vector.hpp"

class PointLight : public Light
{
  public:
	PointLight();
	PointLight(const Vector<3> &position);
	PointLight(const Vector<3> &position, const Vector<3> &color, float brightness = 1.0F);

	void incrementTranslation(float distance, EAxis axis);
	void rotate(float angle, EAxis axis);

	Vector<3> getPosition() const;

  private:
	Vector<3> _position;
};
