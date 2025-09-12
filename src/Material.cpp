
#include "Material.hpp"
#include "Texture.hpp"

Material::Material()
    : _diffuseColor({1, 1, 1}), _ambientColor({1, 1, 1}), _specularColor({1, 1, 1}), _specularExponent(0),
      _texture(nullptr), _normalMap(nullptr), _textureScaling({1, 1})
{
}

Material::Material(const std::string &name)
    : _name(name), _diffuseColor({1, 1, 1}), _ambientColor({1, 1, 1}), _specularColor({1, 1, 1}), _specularExponent(0),
      _texture(nullptr), _normalMap(nullptr), _textureScaling({1, 1})
{
}

Material::Material(const Material &other)
{
	std::cout << "MATERIAL COPY CONSTRUCTOR\n";
	_name = other._name;
	_diffuseColor = other._diffuseColor;
	_ambientColor = other._ambientColor;
	_specularColor = other._specularColor;
	_specularExponent = other._specularExponent;
	_texture = other._texture;
	_normalMap = other._normalMap;
	_textureScaling = other._textureScaling;
}

Material &Material::operator=(const Material &other)
{
	std::cout << "MATERIAL OPERATOR=\n";
	if (this != &other)
	{
		_name = other._name;
		_diffuseColor = other._diffuseColor;
		_ambientColor = other._ambientColor;
		_specularColor = other._specularColor;
		_specularExponent = other._specularExponent;
		_texture = other._texture;
		_normalMap = other._normalMap;
		_textureScaling = other._textureScaling;
	}
	return *this;
}

void Material::draw()
{
	if (_texture)
		_texture->draw(GL_TEXTURE0);
	if (_normalMap)
		_normalMap->draw(GL_TEXTURE1);
}

const std::string &Material::getName() const
{
	return _name;
}

const Vector<3> &Material::getDiffuse() const
{
	return _diffuseColor;
}

const Vector<3> &Material::getAmbient() const
{
	return _ambientColor;
}

const Vector<3> &Material::getSpecular() const
{
	return _specularColor;
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

bool Material::hasNormalMap() const
{
	return _normalMap != nullptr;
}

void Material::setDiffuseColor(const Vector<3> &diffuseColor)
{
	_diffuseColor = diffuseColor;
}

void Material::setAmbientColor(const Vector<3> &ambiantColor)
{
	_ambientColor = ambiantColor;
}

void Material::setSpecularColor(const Vector<3> &specularColor)
{
	_specularColor = specularColor;
}

void Material::setSpecularExponent(float specularExponent)
{
	_specularExponent = specularExponent;
}

void Material::setTexture(Texture *texture)
{
	_texture = texture;
}

void Material::setNormalMap(Texture *normalMap)
{
	_normalMap = normalMap;
}

void Material::setTextureScaling(const Vector<2> &textureScaling)
{
	_textureScaling = textureScaling;
}
