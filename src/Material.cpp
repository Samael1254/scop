
#include "Material.hpp"

Material::Material() : _diffuse({0.969, 0.925, 0.6}), _ambient({1.0, 0.0, 0.0}), _textureID(-1)
{
	(void)_textureID;
}

const Vector<3> &Material::getDiffuse() const
{
	return _diffuse;
}

const Vector<3> &Material::getAmbient() const
{
	return _ambient;
}
