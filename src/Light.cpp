#include "Light.hpp"

Light::Light() : _color({1.0F, 1.0F, 1.0F}), _brightness(1.0F), _position({0.0F, 0.0F, 0.0F}) {}

Light::Light(const Vector<3> &position) : _color({1.0F, 1.0F, 1.0F}), _brightness(1.0F), _position(position) {}

Vector<3> Light::getColor() const
{
	return _color;
}
float Light::getBrightness() const
{
	return _brightness;
}
Vector<3> Light::getPosition() const
{
	return _position;
}
