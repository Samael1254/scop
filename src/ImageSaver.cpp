#include "ImageSaver.hpp"
#include "Texture.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

void ImageSaver::saveFrame(const std::string &screenshotDir, int width, int height)
{
	_createScreenshotDir(screenshotDir);
	std::string   filename = _getValidFilename(screenshotDir);
	std::ofstream os(filename);
	if (!os.is_open())
		throw std::runtime_error("could not create file: " + filename);
	_writeFrameToBMP(os, width, height);
	os.close();
}

void ImageSaver::_createScreenshotDir(const std::string &screenshotDir)
{
	struct stat st;

	if (stat(screenshotDir.c_str(), &st) == -1)
	{
		if (mkdir(screenshotDir.c_str(), 0777) == -1)
			throw std::runtime_error("failed to create directory " + screenshotDir);
	}
	else if (S_ISREG(st.st_mode))
		throw std::runtime_error("could not create " + screenshotDir +
		                         " directory: a file by that name already exists");
}

std::string ImageSaver::_getValidFilename(const std::string &screenshotDir)
{
	std::string filename = screenshotDir + "/screenshot_0.bmp";
	int         i = 1;
	while (access((filename).c_str(), F_OK) == 0)
	{
		std::stringstream ss;
		ss << screenshotDir << "/screenshots_" << i << ".bmp";
		filename = ss.str();
		i++;
	}
	return filename;
}

void ImageSaver::_writeFrameToBMP(std::ofstream &os, int width, int height)
{
	_writeBMPHeader(os, width, height);
	_writeDIBHeader(os, width, height);
	_writePixelData(os, width, height);
}

void ImageSaver::_writeBMPHeader(std::ofstream &os, int width, int height)
{
	os << "BM";
	_writeUInt32(os, _getFileSize(width, height));
	_writeUInt32(os, 0);
	_writeUInt32(os, 54);
}

void ImageSaver::_writeDIBHeader(std::ofstream &os, int width, int height)
{
	_writeUInt32(os, 40);     // header_size
	_writeUInt32(os, width);  // image width in pixels
	_writeUInt32(os, height); // image height in pixels
	_writeUInt16(os, 1);      // number of planes
	_writeUInt16(os, 24);     // bits per pixel
	_writeUInt32(os, 0);      // compression (0 = no compression)
	_writeUInt32(os, 0);      // compressed size
	_writeUInt32(os, 2835);   // horizontal resolution
	_writeUInt32(os, 2835);   // vertical resolution
	_writeUInt32(os, 0);      // number of colors (0 if 24 bpp)
	_writeUInt32(os, 0);      // important colors (0 = all)
}

void ImageSaver::_writePixelData(std::ofstream &os, int width, int height)
{
	const uint32_t       bufSize = width * height * _bytesPerPixel;
	std::vector<uint8_t> buf(bufSize);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buf.data());
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
			_writeColor(os, _getPixelColor(buf.data(), x, y, width, height));
		for (int i = 0; (width * 3 + i) % 4 != 0; i++)
			os << static_cast<uint8_t>(0);
	}
}

void ImageSaver::_writeUInt32(std::ofstream &os, uint32_t value)
{
	for (int i = 0; i < 4; ++i)
		os << static_cast<uint8_t>((value >> 8 * i) & 0xFF);
}

void ImageSaver::_writeUInt16(std::ofstream &os, uint16_t value)
{
	for (int i = 0; i < 2; ++i)
		os << static_cast<uint8_t>((value >> 8 * i) & 0xFF);
}

void ImageSaver::_writeColor(std::ofstream &os, const std::array<uint8_t, 3> &color)
{
	for (int i = 0; i < 3; ++i)
		os << static_cast<uint8_t>(color[2 - i]);
}

uint32_t ImageSaver::_getFileSize(int width, int height)
{
	const uint32_t rowSize = ((width * 3 + 3) / 4) * 4;
	return _headerSize + rowSize * height;
}

std::array<uint8_t, 3> ImageSaver::_getPixelColor(const uint8_t *buf, int x, int y, int width, int height)
{
	(void)height;
	std::array<uint8_t, 3> color;
	for (int i = 0; i < 3; ++i)
		color[i] = buf[(y * width + x) * _bytesPerPixel + i];
	return color;
}
