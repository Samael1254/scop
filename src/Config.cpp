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

const Config::Camera &Config::getCamera() const
{
	return _camera;
}

const Config::Object &Config::getObject() const
{
	return _object;
}

const Config::Light &Config::getLight() const
{
	return _light;
}
const Config::Ambiant &Config::getAmbiant() const
{
	return _ambiant;
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
	std::cout << "\n[Camera]\n";
	std::cout << "fov = " << _camera.fov << "\n";
	std::cout << "\n[Object]\n";
	std::cout << "rotationSpeed = " << _object.rotationSpeed << "\n";
	std::cout << "translationSpeed = " << _object.translationSpeed << "\n";
	std::cout << "zoomSpeed = " << _object.zoomSpeed << "\n";
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
		try
		{
			line = TomlParser::parseLine(line);

			if (line.empty() || line[0] == '#')
				continue;
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
			case CAMERA:
				_loadCamera(key, value);
				break;
			case OBJECT:
				_loadObject(key, value);
				break;
			case LIGHT:
				_loadLight(key, value);
				break;
			case AMBIANT:
				_loadAmbiant(key, value);
				break;
			}
		}
		catch (std::exception &e)
		{
			std::cerr << "\e[033mConfig warning:\e[0m " << e.what() << std::endl;
		}
	}
}

Config::Table Config::_switchTable(std::string &line)
{
	if (!line.ends_with(']'))
		throw std::runtime_error("bad table syntax (expected ']' at the end): " + line);
	line = line.substr(1, line.length() - 2);
	line.erase(0, line.find_first_not_of(" \t"));
	line.erase(line.find_last_not_of(" \t") + 1);
	if (line == "Window")
		return Table::WINDOW;
	if (line == "Background")
		return Table::BACKGROUND;
	if (line == "Camera")
		return Table::CAMERA;
	if (line == "Object")
		return Table::OBJECT;
	if (line == "Light")
		return Table::LIGHT;
	if (line == "Ambiant")
		return Table::AMBIANT;
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

void Config::_loadCamera(const std::string &key, const std::string &value)
{
	if (key == "fov")
		_camera.fov = TomlParser::readFloat(key, value);
	else
		throw std::runtime_error("unknown key in 'Camera': " + key);
}

void Config::_loadObject(const std::string &key, const std::string &value)
{
	if (key == "rotationSpeed")
		_object.rotationSpeed = TomlParser::readFloat(key, value);
	else if (key == "translationSpeed")
		_object.translationSpeed = TomlParser::readFloat(key, value);
	else if (key == "zoomSpeed")
		_object.zoomSpeed = TomlParser::readFloat(key, value);
	else
		throw std::runtime_error("unknown key in 'Object': " + key);
}

void Config::_loadLight(const std::string &key, const std::string &value)
{
	if (key == "color")
		_light.color = TomlParser::readColor(key, value);
	else if (key == "brightness")
		_light.brightness = TomlParser::readFloat(key, value);
	else
		throw std::runtime_error("unknown key in 'Light': " + key);
}

void Config::_loadAmbiant(const std::string &key, const std::string &value)
{
	if (key == "color")
		_ambiant.color = TomlParser::readColor(key, value);
	else if (key == "brightness")
		_ambiant.brightness = TomlParser::readFloat(key, value);
	else
		throw std::runtime_error("unknown key in 'Ambiant': " + key);
}
