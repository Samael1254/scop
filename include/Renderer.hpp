#pragma once

#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Model.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <vector>

class Renderer
{
  public:
	Renderer(int width, int height, Model &model);
	Renderer(const Renderer &other) = default;
	~Renderer() = default;

	Renderer &operator=(const Renderer &other);

	void init();
	void render();

	void        switchPolygonMode();
	static void toggleAntialiasing();
	void        toggleShowTriangles();

	void updateModel();
	void updateLight();
	void resize(int width, int height);

	Camera     &getCamera();
	Model      &getModel();
	Shader     &getShader();
	PointLight &getLight();
	float       getRotationSpeed() const;
	float       getTranslationSpeed() const;
	float       getZoomSpeed() const;

  private:
	Model                 _model;
	Shader                _shader;
	std::vector<Material> _materials;
	std::vector<Texture>  _textures;

	Camera     _camera;
	PointLight _light;
	Light      _ambiantLight;

	bool _showTriangles;
	int  _polygonMode;

	float _rotationSpeed;
	float _translationSpeed;
	float _zoomSpeed;

	static Matrix<3, 3> _normalMatrix(const Matrix<4, 4> &modelMatrix, const Matrix<4, 4> &viewMatrix);
};
