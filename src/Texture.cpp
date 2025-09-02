#include "Texture.hpp"
#include <vector>

Texture::Texture(const std::string &filepath)
{
	(void)filepath;
}

Texture::Texture(const Texture &other)
{
	(void)other;
}

Texture::~Texture() {}

std::vector<unsigned char> Texture::_loadBMP(const std::string &filepath)
{
	(void)filepath;
	return std::vector<unsigned char>();
}
