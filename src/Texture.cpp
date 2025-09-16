#include "Texture.hpp"
#include <array>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>

Texture::Texture(const std::string &filepath)
{
	_loadBMP(filepath);
	_setup();
}

Texture::Texture(const Texture &other)
{
	_pixelData = other._pixelData;
	_palette = other._palette;
	_imageInfo = other._imageInfo;
	_setup();
}

void Texture::draw(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _id);
}

const uint8_t *Texture::data() const
{
	return _pixelData.data();
}

int Texture::width() const
{
	return static_cast<int>(_imageInfo.width);
}

int Texture::height() const
{
	return static_cast<int>(_imageInfo.height);
}

void Texture::_loadBMP(const std::string &filepath)
{
	std::ifstream is(filepath);
	if (!is.is_open())
		throw std::runtime_error("failed to open .bmp file: " + filepath);

	_readHeaders(is);
	if (_imageInfo.bitsPerPixel <= 8)
		_readPalette(is);
	_readPixelData(is);
	is.close();
}

void Texture::_readHeaders(std::istream &is)
{
	_readUInt16(is);
	_imageInfo.fileSize = _readUInt32(is);
	_readUInt32(is);
	_imageInfo.offset = _readUInt32(is);
	_imageInfo.headerSize = _readUInt32(is);
	_imageInfo.width = static_cast<int>(_readUInt32(is));
	_imageInfo.height = static_cast<int>(_readUInt32(is));
	_imageInfo.planes = _readUInt16(is);
	_imageInfo.bitsPerPixel = _readUInt16(is);
	_imageInfo.compression = _readUInt32(is);
	_imageInfo.imageSize = _readUInt32(is);
	_imageInfo.xPixelsPerM = static_cast<int>(_readUInt32(is));
	_imageInfo.yPixelsPerM = static_cast<int>(_readUInt32(is));
	_imageInfo.colorsUsed = _readUInt32(is);
	_imageInfo.importantColors = _readUInt32(is);

	if (_imageInfo.compression != 0)
		throw std::runtime_error("cannot load compressed texture");

	is.seekg(_imageInfo.headerSize - 40, std::ios_base::cur);
}

void Texture::_readPalette(std::istream &is)
{
	for (unsigned int i = 0; i < _imageInfo.colorsUsed; ++i)
	{
		std::array<uint8_t, 3> color;
		for (unsigned int j = 0; j < 3; ++j)
			color[2 - j] = _readUInt32(is);
		_readUInt32(is);
		_palette.push_back(color);
	}
}

void Texture::_readPixelData(std::istream &is)
{
	for (int i = 0; i < _imageInfo.height; ++i)
	{
		int j = 0;
		while (j < _imageInfo.bitsPerPixel / 8 * _imageInfo.width)
		{
			if (_palette.empty())
			{
				std::array<uint8_t, 3> color;
				for (int k = 0; k < 3; k++)
					color[k] = _readUInt8(is);
				for (int k = 0; k < 3; k++)
					_pixelData.push_back(color[2 - k]);
				j += 3;
			}
			else
				for (unsigned int k = 0; k < 3; ++k)
					_pixelData.push_back(_palette[_readUInt8(is)][k]);
		}
		while (j++ < ((_imageInfo.bitsPerPixel * _imageInfo.width + 31) / 32) * 4)
			_readUInt8(is);
	}
}

uint32_t Texture::_readUInt32(std::istream &is)
{
	char bytes[4];
	is.read(bytes, 4);
	uint32_t value = *reinterpret_cast<uint32_t *>(bytes);
	return value;
}

uint16_t Texture::_readUInt16(std::istream &is)
{
	char bytes[2];
	is.read(bytes, 2);
	uint16_t value = *reinterpret_cast<uint16_t *>(bytes);
	return value;
}

uint8_t Texture::_readUInt8(std::istream &is)
{
	char bytes[1];
	is.read(bytes, 1);
	uint16_t value = *reinterpret_cast<uint16_t *>(bytes);
	return value;
}

void Texture::_setup()
{
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
