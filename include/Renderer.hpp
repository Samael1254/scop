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
#include <cstdint>
#include <glad/gl.h>

class Renderer
{
  public:
	Renderer(int width, int height, Model *model, Texture *texture);
	Renderer(const Renderer &other) = default;
	~Renderer() = default;

	Renderer &operator=(const Renderer &other);

	void render();

	void        switchPolygonMode();
	static void toggleAntialiasing();
	void        changeDisplayMode();

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

	enum DisplayMode : uint8_t
	{
		REGULAR,
		TRIANGLE,
		TEXTURE,
		NORMALS,
	};

  private:
	Model    *_model;
	Material *_material;
	Texture  *_texture;
	Shader    _shader;

	Camera     _camera;
	PointLight _light;
	Light      _ambiantLight;

	DisplayMode _displayMode;
	int         _polygonMode;

	float _rotationSpeed;
	float _translationSpeed;
	float _zoomSpeed;

	void                _init();
	static Matrix<3, 3> _normalMatrix(const Matrix<4, 4> &modelMatrix, const Matrix<4, 4> &viewMatrix);
};
