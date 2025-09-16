#include "PointLight.hpp"
#include "Light.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"

PointLight::PointLight() : _position({-5.0F, 0.0F, -0.0F}) {}

PointLight::PointLight(const Vector<3> &position) : _position(position) {}

PointLight::PointLight(const Vector<3> &position, const Vector<3> &color, float brightness)
    : Light(color, brightness), _position(position)
{
}

void PointLight::incrementTranslation(float distance, EAxis axis)
{
	_position[axis] += distance;
}

void PointLight::rotate(float angle, EAxis axis)
{
	Matrix<4, 4> rot = Matrix<4, 4>::createRotationMatrix(angle, axis);
	Vector<4>    pos4 = _position.resize<4>(true);

	pos4 = rot * pos4;
	_position = pos4.resize<3>(false);
}

Vector<3> PointLight::getPosition() const
{
	return _position;
}
