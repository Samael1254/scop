#pragma once

#include "Camera.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>

class Renderer
{
  public:
	Renderer(int width, int height);
	~Renderer() = default;

	void render();
	void setPolygonMode(int polygonMode);

  private:
	Model                 _model;
	Shader                _shader;
	std::vector<Material> _materials;
	std::vector<Texture>  _textures;
	Camera                _camera;
	int                   _polygonMode;

	void _init();
};
