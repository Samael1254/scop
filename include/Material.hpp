#pragma once

#include "Texture.hpp"
#include "Vector.hpp"
#include <string>
#include <vector>

class Material
{
  public:
	Material();
	Material(const std::string &name);
	Material(const Material &other);

	Material &operator=(const Material &other);

	void draw();

	const std::string &getName() const;
	const Vector<3>   &getDiffuse() const;
	const Vector<3>   &getAmbient() const;
	const Vector<3>   &getSpecular() const;
	float              getSpecularExponent() const;
	const Texture     *getTexture() const;
	const Vector<2>   &getTextureScaling() const;

	bool hasTexture() const;
	bool hasNormalMap() const;

	void setDiffuseColor(const Vector<3> &diffuseColor);
	void setAmbientColor(const Vector<3> &ambiantColor);
	void setSpecularColor(const Vector<3> &specularColor);
	void setSpecularExponent(float specularExponent);
	void setTexture(Texture *texture);
	void setNormalMap(Texture *normalMap);
	void setTextureScaling(const Vector<2> &textureScaling);

	static std::vector<Material> parseMTL(const std::string &filepath);

  private:
	std::string _name;

	Vector<3> _diffuseColor;
	Vector<3> _ambientColor;
	Vector<3> _specularColor;
	float     _specularExponent;

	Texture  *_texture;
	Texture  *_normalMap;
	Vector<2> _textureScaling;
};
