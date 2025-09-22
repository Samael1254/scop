#include "Config.hpp"
#include "TomlParser.hpp"
#include <cctype>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>

Config::Config()
{
	try
	{
		_loadConfig("scop.toml");
		print();
	}
	catch (std::exception &e)
	{
		std::cerr << "\e[033mConfig warning:\e[0m " << e.what() << std::endl;
	}
}

const Config::Root &Config::getRoot() const
{
	return _root;
}

const Config::Window &Config::getWindow() const
{
	return _window;
}

const Config::Background &Config::getBackground() const
{
	return _background;
}

void Config::print() const
{
	std::cout << std::boolalpha;
	std::cout << "-- Config scop.toml --\n";
	std::cout << "\n[Root]\n";
	std::cout << "verbose = " << _root.verbose << "\n";
	std::cout << "antialiasing = " << _root.antialiasing << "\n";
	std::cout << "\n[Window]\n";
	std::cout << "width = " << _window.width << "\n";
	std::cout << "height = " << _window.height << "\n";
	std::cout << "fullscreen = " << _window.fullscreen << "\n";
	std::cout << "\n[Background]\n";
	std::cout << "color = ";
	std::cout << _background.color[0] << ", ";
	std::cout << _background.color[1] << ", ";
	std::cout << _background.color[2] << "\n";
	std::cout << std::noboolalpha;
}

void Config::_loadConfig(const std::string &confFile)
{
	std::ifstream is(confFile);
	if (!is.is_open())
		throw std::runtime_error("no config file found (scop.toml)");
	Table       table = ROOT;
	std::string line;
	while (std::getline(is, line))
	{
		line = TomlParser::parseLine(line);

		if (line.empty() || line[0] == '#')
			continue;
		std::cout << line << "\n";
		if (line[0] == '[')
		{
			table = _switchTable(line);
			continue;
		}

		std::string key = TomlParser::parseKey(line);
		std::string value = TomlParser::parseValue(line);

		switch (table)
		{
		case ROOT:
			_loadRoot(key, value);
			break;
		case WINDOW:
			_loadWindow(key, value);
			break;
		case BACKGROUND:
			_loadBackground(key, value);
			break;
		}
	}
}

Config::Table Config::_switchTable(std::string &line)
{
	if (!line.ends_with(']'))
		throw std::runtime_error("bad table syntax (expected ']' at the end: " + line);
	line = line.substr(1, line.length() - 2);
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);
	if (line == "Window")
		return Table::WINDOW;
	if (line == "Background")
		return Table::BACKGROUND;
	throw std::runtime_error("unknown table value: " + line);
}

void Config::_loadRoot(const std::string &key, const std::string &value)
{
	if (key == "verbose")
		_root.verbose = TomlParser::readBool(key, value);
	else if (key == "antialiasing")
		_root.antialiasing = TomlParser::readBool(key, value);
	else
		throw std::runtime_error("unknown key in 'Root': " + key);
}

void Config::_loadWindow(const std::string &key, const std::string &value)
{
	if (key == "width")
		_window.width = static_cast<int>(TomlParser::readUInt(key, value));
	else if (key == "height")
		_window.height = static_cast<int>(TomlParser::readUInt(key, value));
	else if (key == "fullscreen")
		_window.fullscreen = TomlParser::readBool(key, value);
	else
		throw std::runtime_error("unknown key in 'Window': " + key);
}

void Config::_loadBackground(const std::string &key, const std::string &value)
{
	if (key == "color")
		_background.color = TomlParser::readColor(key, value);
	else
		throw std::runtime_error("unknown key in 'Background': " + key);
}
