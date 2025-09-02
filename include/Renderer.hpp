#pragma once

#include "Material.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <vector>

class Renderer
{
  public:
	Renderer();
	~Renderer() = default;

	void render();
	void setPolygonMode(int polygonMode);

  private:
	Model                 _model;
	Shader                _shader;
	std::vector<Material> _materials;
	std::vector<Texture>  _textures;

	int _polygonMode;

	void _init();
};
