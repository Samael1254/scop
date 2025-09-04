#pragma once

#include "Camera.hpp"
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
	Renderer(int width, int height);
	Renderer(const Renderer &other) = default;
	~Renderer() = default;

	Renderer &operator=(const Renderer &other);

	void init();
	void render();

	void switchPolygonMode();
	void updateModel();

	Camera &getCamera();
	Model  &getModel();
	float   getRotationSpeed() const;
	float   getZoomSpeed() const;

  private:
	Model                 _model;
	Shader                _shader;
	std::vector<Material> _materials;
	std::vector<Texture>  _textures;
	Camera                _camera;
	int                   _polygonMode;

	float _rotationSpeed;
	float _zoomSpeed;
};
