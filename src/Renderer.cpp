#include "Renderer.hpp"
#include "Shader.hpp"
#include "liblinal.hpp"
#include <GLFW/glfw3.h>
#include <cmath>
#include <stdexcept>
#include <string>

Renderer::Renderer(int width, int height, Model &model)
    : _model(model), _shader(Shader("vertexShader.vert", "fragmentShader.frag")), _camera(width, height),
      _polygonMode(GL_LINE), _rotationSpeed(0.03), _zoomSpeed(0.1)
{
	init();
}

Renderer &Renderer::operator=(const Renderer &other)
{
	if (this != &other)
	{
		_model = other._model;
		_shader = other._shader;
		_materials = other._materials;
		_textures = other._textures;
		_camera = other._camera;
		_polygonMode = other._polygonMode;
		_rotationSpeed = other._rotationSpeed;
		_zoomSpeed = other._zoomSpeed;
	}
	return *this;
}

void Renderer::render()
{
	glClearColor(0.102, 0.188, 0.29, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_model.draw(_shader);
}

void Renderer::switchPolygonMode()
{
	switch (_polygonMode)
	{
	case GL_POINT:
		_polygonMode = GL_LINE;
		break;
	case GL_LINE:
		_polygonMode = GL_FILL;
		break;
	case GL_FILL:
		_polygonMode = GL_POINT;
		break;
	default:
		throw std::logic_error("invalid polygon mode");
		break;
	}
	glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);
}

Camera &Renderer::getCamera()
{
	return _camera;
}

Model &Renderer::getModel()
{
	return _model;
}

float Renderer::getRotationSpeed() const
{
	return _rotationSpeed;
}

float Renderer::getZoomSpeed() const
{
	return _zoomSpeed;
}

void Renderer::updateModel()
{
	_shader.setUniform("model", _model.matrix());
}

void Renderer::init()
{
	// Setup model in scene
	_model.incrementRotation(M_PI, YAxis);
	_model.incrementScale(1);

	// _model.center();

	// Setshader uniforms
	_shader.use();
	_shader.setUniform("proj", _camera.projectionMatrix());
	_shader.setUniform("view", _camera.viewMatrix());
	_shader.setUniform("model", _model.matrix());

	// OpenGL parameters
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);
}
