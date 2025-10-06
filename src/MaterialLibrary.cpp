#include "MaterialLibrary.hpp"
#include "Engine.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Vector.hpp"
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <stdexcept>
#include <string>

MaterialLibrary::MaterialLibrary()
{
	_materials.emplace_back();
}

MaterialLibrary::MaterialLibrary(const std::string &filepath)
{
	_materials.emplace_back();
	_loadMTL(filepath);
}

MaterialLibrary::MaterialLibrary(MaterialLibrary &&other) noexcept
{
	_textures = std::move(other._textures);
	_materials = std::move(other._materials);
}

MaterialLibrary &MaterialLibrary::operator=(MaterialLibrary &&other) noexcept
{
	if (this != &other)
	{
		_textures = std::move(other._textures);
		_materials = std::move(other._materials);
	}
	return *this;
}

Material *MaterialLibrary::getDefaultMaterial()
{
	return &_materials.front();
}

Material *MaterialLibrary::getLastMaterial()
{
	return &_materials.back();
}

Material *MaterialLibrary::getMaterial(const std::string &name)
{
	auto cmpName = [&name](const Material &mat) { return mat.getName() == name; };

	std::deque<Material>::iterator it = std::find_if(_materials.begin(), _materials.end(), cmpName);
	if (it == _materials.end())
		throw std::runtime_error("no such material in material library: " + name);
	return &*it;
}

void MaterialLibrary::_loadMTL(const std::string &filepath)
{
	std::ifstream is = Engine::openFile(filepath, "mtl");

	std::string line;
	while (std::getline(is, line))
		while (line.substr(0, 7) == "newmtl ")
			_readMaterial(is, line);
	is.close();
}

void MaterialLibrary::_readMaterial(std::ifstream &is, std::string &line)
{
	_materials.emplace_back(line.substr(7));
	Material &newmtl = _materials.back();
	while (std::getline(is, line) && line.substr(0, 7) != "newmtl ")
	{
		unsigned long spaceIndex = line.find(' ');
		std::string   qualifier = line.substr(0, spaceIndex);
		line = line.substr(spaceIndex + 1);
		if (qualifier == "Ka")
			newmtl.setAmbientColor(_readColor(line));
		if (qualifier == "Kd")
			newmtl.setDiffuseColor(_readColor(line));
		if (qualifier == "Ks")
			newmtl.setSpecularColor(_readColor(line));
		if (qualifier == "Ns")
			newmtl.setSpecularExponent(static_cast<float>(std::atof(line.c_str())));
		if (qualifier == "map_Kd")
			newmtl.setTexture(_readTexture(line));
		if (qualifier == "bump")
			newmtl.setNormalMap(_readTexture(line));
	}
}

Vector<3> MaterialLibrary::_readColor(const std::string &data)
{
	Vector<3>     color;
	unsigned long startIndex = 0;
	for (int i = 0; i < 3; ++i)
	{
		unsigned long spaceIndex = data.find(' ', startIndex);
		color[i] = static_cast<float>(std::atof(data.substr(startIndex, spaceIndex - startIndex).c_str()));
		startIndex = spaceIndex + 1;
	}
	return color;
}

Texture *MaterialLibrary::_readTexture(const std::string &data)
{
	std::string filepath = data.substr(data.find_last_of(' ', data.find_last_not_of(' ')) + 1);
	_textures.emplace_back(filepath);
	return &_textures.back();
}
