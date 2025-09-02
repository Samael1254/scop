#pragma once

#include <string>
#include <vector>
class Texture
{
  public:
	Texture(const std::string &filepath);
	Texture(const Texture &other);
	~Texture();

  private:
	std::vector<unsigned char> _pixels;

	std::vector<unsigned char> _loadBMP(const std::string &filepath);
};
