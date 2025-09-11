
#include "Material.hpp"

Material::Material()
    : _diffuse({0.969, 0.925, 0.6}), _ambient({0.5, 0, 1}), _specular({1, 1, 1}), _specularExponent(100),
      _texture(nullptr), _textureScaling({0.5, 0.5})
{
}

Material::Material(Texture *texture)
    : _diffuse({1, 1, 1}), _ambient({1.0, 1.0, 1.0}), _specular({1, 1, 1}), _specularExponent(100), _texture(texture),
      _textureScaling({0.5, 0.5})
{
}

const Vector<3> &Material::getDiffuse() const
{
	return _diffuse;
}

const Vector<3> &Material::getAmbient() const
{
	return _ambient;
}

const Vector<3> &Material::getSpecular() const
{
	return _specular;
}

float Material::getSpecularExponent() const
{
	return _specularExponent;
}

const Texture *Material::getTexture() const
{
	return _texture;
}

const Vector<2> &Material::getTextureScaling() const
{
	return _textureScaling;
}

bool Material::hasTexture() const
{
	return _texture != nullptr;
}

void Material::setTexture(Texture *texture)
{
	_texture = texture;
}
