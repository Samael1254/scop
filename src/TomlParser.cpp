#include "TomlParser.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string>

std::string TomlParser::parseLine(const std::string &line)
{
	if (line.empty())
		return line;
	std::string str = _stripComment(line);
	str.erase(0, str.find_first_not_of(" \t"));
	str.erase(str.find_last_not_of(" \t") + 1);
	return str;
}

std::string TomlParser::parseKey(const std::string &line)
{
	std::string key = line.substr(0, line.find_first_of('=', 0));
	key.erase(key.find_last_not_of(" \t") + 1);
	return key;
}

std::string TomlParser::parseValue(const std::string &line)
{
	std::string value = line.substr(line.find_first_of('=', 0) + 1);
	value.erase(0, value.find_first_not_of(" \t"));
	return value;
}

bool TomlParser::readBool(const std::string &key, const std::string &value)
{
	if (value == "true")
		return true;
	if (value == "false")
		return false;
	throw std::runtime_error("invalid value for '" + key + "': " + value);
}
uint32_t TomlParser::readUInt(const std::string &key, const std::string &value)
{
	if (value.empty() ||
	    std::find_if(value.begin(), value.end(), [](unsigned char c) { return !std::isdigit(c); }) != value.end())
		throw std::runtime_error("invalid value for '" + key + "' (expected positive integer): " + value);
	return std::atoi(value.c_str());
}

float TomlParser::readFloat(const std::string &key, const std::string &value)
{
	if (value.empty() || std::find_if(value.begin(), value.end(),
	                                  [](unsigned char c) { return !std::isdigit(c) && c != '.'; }) != value.end())
		throw std::runtime_error("invalid value for '" + key + "' (expected positive integer): " + value);
	return static_cast<float>(std::atof(value.c_str()));
}

std::string TomlParser::readString(const std::string &key, const std::string &value)
{
	if ((!value.starts_with('\'') || !value.ends_with('\'')) && (!value.starts_with('"') || !value.ends_with('"')))
		throw std::runtime_error("invalid value for '" + key + "' (string must be surrounded by quotes): " + value);
	return value.substr(1, value.length() - 2);
}

std::array<float, 3> TomlParser::readColor(const std::string &key, const std::string &value)
{
	std::string str = readString(key, value);
	if (str[0] != '#')
		throw std::runtime_error("hex color value must start with '#' in " + key);
	if (str.length() != 7)
		throw std::runtime_error("hex color value must be 6 characters long: " + key);
	int                  colorRaw = std::stoi(str.substr(1), nullptr, 16);
	std::array<float, 3> color;
	color[0] = static_cast<float>((colorRaw >> 16) & 0xFF) / 255.0F;
	color[1] = static_cast<float>((colorRaw >> 8) & 0xFF) / 255.0F;
	color[2] = static_cast<float>(colorRaw & 0xFF) / 255.0F;
	return color;
}

std::string TomlParser::_stripComment(const std::string &line)
{
	bool inDouble = false;
	bool inSingle = false;

	for (uint32_t i = 0; i < line.length(); i++)
	{
		if (line[i] == '\"' && !inSingle)
			inDouble = !inDouble;
		if (line[i] == '\'' && !inDouble)
			inSingle = !inSingle;
		if (line[i] == '#' && !inSingle && !inDouble)
			return line.substr(0, i);
	}
	return line;
}
