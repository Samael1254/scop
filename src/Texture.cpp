#include "Texture.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

Texture::Texture(const std::string &filepath)
{
	_pixelData = _loadBMP(filepath);
}

Texture::Texture(const Texture &other)
{
	(void)other;
}

const unsigned char *Texture::data() const
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

std::vector<unsigned char> Texture::_loadBMP(const std::string &filepath)
{
	std::ifstream is(filepath);
	if (!is.is_open())
		throw std::runtime_error("failed to open .obj file: " + filepath);
	_readHeaders(is);
	if (_imageInfo.bitsPerPixel <= 8)
		_readPalette(is);

	return std::vector<unsigned char>();
}

void Texture::_readHeaders(std::istream &is)
{
	_readUInt16(is);
	_imageInfo.fileSize = _readUInt32(is);
	_readUInt32(is);
	_imageInfo.offset = _readUInt32(is);
	_imageInfo.headerSize = _readUInt32(is);
	_imageInfo.width = _readUInt32(is);
	_imageInfo.height = _readUInt32(is);
	_imageInfo.planes = _readUInt16(is);
	_imageInfo.bitsPerPixel = _readUInt16(is);
	_imageInfo.compression = _readUInt32(is);
	_imageInfo.imageSize = _readUInt32(is);
	_imageInfo.xPixelsPerM = _readUInt32(is);
	_imageInfo.yPixelsPerM = _readUInt32(is);
	_imageInfo.colorsUsed = _readUInt32(is);
	_imageInfo.importantColors = _readUInt32(is);

	std::cout << "File size: " << _imageInfo.fileSize << "\n";
	std::cout << "Offset: " << _imageInfo.offset << "\n";
	std::cout << "Header size: " << _imageInfo.headerSize << "\n";
	std::cout << "Width: " << _imageInfo.width << "\n";
	std::cout << "Height: " << _imageInfo.height << "\n";
	std::cout << "Planes: " << _imageInfo.planes << "\n";
	std::cout << "Bits Per Pixel: " << _imageInfo.bitsPerPixel << "\n";
	std::cout << "Planes: " << _imageInfo.planes << "\n";
	std::cout << "Compression: " << _imageInfo.compression << "\n";
	std::cout << "Image Size: " << _imageInfo.imageSize << "\n";
	std::cout << "X pixels per meter: " << _imageInfo.xPixelsPerM << "\n";
	std::cout << "Y pixels per meter: " << _imageInfo.yPixelsPerM << "\n";
	std::cout << "Colors used: " << _imageInfo.colorsUsed << "\n";
	std::cout << "Important colors: " << _imageInfo.importantColors << "\n";
	is.seekg(_imageInfo.headerSize - 40);
}

void Texture::_readPalette(std::istream &is)
{
	for (unsigned int i = 0; i < _imageInfo.colorsUsed; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
			_palette[i][j] = _readUInt32(is);
		_readUInt32(is);
	}
}

void Texture::_readPixelData(std::istream &is)
{
	for (unsigned int i = 0; i < _imageInfo.height; ++i)
	{
		for (unsigned int j = 0; j < _imageInfo.width; ++j)
		{
			uint32_t byte = _readUInt32(is);
			if (_palette.empty())
				_pixelData.push_back(byte);
			else
				for (unsigned int k = 0; k < 3; ++k)
					_pixelData.push_back(_palette[byte][k]);
		}
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
