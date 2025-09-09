#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>
class Texture
{
  public:
	Texture(const std::string &filepath);
	Texture(const Texture &other);
	~Texture() = default;

	const unsigned char *data() const;
	int                  width() const;
	int                  height() const;

  private:
	std::vector<unsigned char>                _pixelData;
	std::vector<std::array<unsigned char, 3>> _palette;

	struct ImageInfo
	{
		uint32_t fileSize;
		uint32_t offset;
		uint32_t headerSize;
		uint32_t width;
		uint32_t height;
		uint32_t planes;
		uint32_t bitsPerPixel;
		uint32_t compression;
		uint32_t imageSize;
		uint32_t xPixelsPerM;
		uint32_t yPixelsPerM;
		uint32_t colorsUsed;
		uint32_t importantColors;
	} _imageInfo;

	std::vector<unsigned char> _loadBMP(const std::string &filepath);
	void                       _readHeaders(std::istream &is);
	void                       _readPalette(std::istream &is);
	void                       _readPixelData(std::istream &is);
	static uint32_t            _readUInt32(std::istream &is);
	static uint16_t            _readUInt16(std::istream &is);
};
