#pragma once

#include "Material.hpp"
#include "Texture.hpp"
#include "Vector.hpp"
#include <deque>
#include <fstream>

class MaterialLibrary
{
  public:
	MaterialLibrary();
	MaterialLibrary(const std::string &filepath);
	MaterialLibrary(MaterialLibrary &&other) noexcept;

	MaterialLibrary &operator=(MaterialLibrary &&other) noexcept;

	Material *getDefaultMaterial();
	Material *getLastMaterial();
	Material *getMaterial(const std::string &name);

  private:
	std::deque<Material> _materials;
	std::deque<Texture>  _textures;

	void             _loadMTL(const std::string &filepath);
	void             _readMaterial(std::ifstream &is, std::string &line);
	static Vector<3> _readColor(const std::string &data);
	Texture         *_readTexture(const std::string &data);
};
