#pragma once

#include "Texture.hpp"
#include "Vector.hpp"

class Material
{
  public:
	Material();
	Material(Texture *texture);

	const Vector<3> &getDiffuse() const;
	const Vector<3> &getAmbient() const;
	const Vector<3> &getSpecular() const;
	float            getSpecularExponent() const;
	const Texture   *getTexture() const;

	bool hasTexture() const;

	void setTexture(Texture *texture);

  private:
	Vector<3> _diffuse;
	Vector<3> _ambient;
	Vector<3> _specular;
	float     _specularExponent;

	Texture *_texture;
};
