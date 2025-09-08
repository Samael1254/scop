#include "Light.hpp"

Light::Light() : _color({1.0F, 1.0F, 1.0F}), _brightness(1.0F) {}

Light::Light(const Vector<3> &color, float brightness) : _color(color), _brightness(brightness) {}

Vector<3> Light::getColor() const
{
	return _color;
}
float Light::getBrightness() const
{
	return _brightness;
}
