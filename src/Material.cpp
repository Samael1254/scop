
#include "Material.hpp"

Material::Material() : _diffuse({0.7, 0.7, 0.7}), _ambient({1.0, 1.0, 1.0}), _textureID(-1)
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
