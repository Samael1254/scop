#pragma once

#include <array>
#include <string>

class TomlParser
{
  public:
	static std::string parseLine(const std::string &line);
	static std::string parseKey(const std::string &line);
	static std::string parseValue(const std::string &line);

	static bool                 readBool(const std::string &key, const std::string &value);
	static uint32_t             readUInt(const std::string &key, const std::string &value);
	static std::string          readString(const std::string &key, const std::string &value);
	static std::array<float, 3> readColor(const std::string &key, const std::string &value);

  private:
	TomlParser() = default;
	TomlParser(const TomlParser &other) = default;
	TomlParser &operator=(const TomlParser &other) = default;

	static std::string _stripComment(const std::string &line);
};
