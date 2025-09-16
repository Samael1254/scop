#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <string>

class ImageSaver
{
  public:
	static void saveFrame(const std::string &screenshotDir, int width, int height);

  private:
	ImageSaver() = default;
	ImageSaver(const ImageSaver &other) = default;
	~ImageSaver() = default;

	static void        _createScreenshotDir(const std::string &screenshotDir);
	static std::string _getValidFilename(const std::string &screenshotDir);

	static void _writeFrameToBMP(std::ofstream &os, int width, int height);
	static void _writeBMPHeader(std::ofstream &os, int width, int height);
	static void _writeDIBHeader(std::ofstream &os, int width, int height);
	static void _writePixelData(std::ofstream &os, int width, int height);

	static void _writeUInt32(std::ofstream &os, uint32_t value);
	static void _writeUInt16(std::ofstream &os, uint16_t value);
	static void _writeColor(std::ofstream &os, const std::array<uint8_t, 3> &color);

	static uint32_t               _getFileSize(int width, int height);
	static std::array<uint8_t, 3> _getPixelColor(const uint8_t *buf, int x, int y, int width, int height);

	static const uint32_t _headerSize = 54;
	static const uint32_t _bytesPerPixel = 3;
};
