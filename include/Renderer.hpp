#pragma once

#include "Material.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <cstdint>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
  public:
	Renderer();
	~Renderer() = default;

	void render();
	void setPolygonMode(int polygonMode);

  private:
	std::vector<uint32_t> _vertexArrayIDs;

	std::vector<Shader>   _shaders;
	std::vector<Model>    _models;
	std::vector<Material> _materials;
	std::vector<Texture>  _textures;

	int _polygonMode;

	void _init();
};
