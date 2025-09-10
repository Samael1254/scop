#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <string>
#include <vector>
class Texture
{
  public:
	Texture();
	Texture(const std::string &filepath);
	Texture(const Texture &other);
	~Texture() = default;

	const uint8_t *data() const;
	int            width() const;
	int            height() const;
	int            empty() const;

  private:
	std::vector<uint8_t>                _pixelData;
	std::vector<std::array<uint8_t, 3>> _palette;
	bool                                _isEmpty;
	struct ImageInfo
	{
		uint32_t fileSize;
		uint32_t offset;
		uint32_t headerSize;
		int      width;
		int      height;
		uint16_t planes;
		uint16_t bitsPerPixel;
		uint32_t compression;
		uint32_t imageSize;
		int      xPixelsPerM;
		int      yPixelsPerM;
		uint32_t colorsUsed;
		uint32_t importantColors;
	} _imageInfo;

	void _loadBMP(const std::string &filepath);
	void _readHeaders(std::istream &is);
	void _readPalette(std::istream &is);
	void _readPixelData(std::istream &is);

	static uint32_t _readUInt32(std::istream &is);
	static uint16_t _readUInt16(std::istream &is);
	static uint8_t  _readUInt8(std::istream &is);
};
