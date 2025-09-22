#include "Renderer.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include <GLFW/glfw3.h>
#include <array>
#include <cmath>
#include <stdexcept>
#include <string>

Renderer::Renderer(const Config &config, Model *model)
    : _config(config), _model(model), _shader(Shader("vertexShader.vert", "fragmentShader.frag")),
      _camera(config.getWindow().width, config.getWindow().height),
      _light(PointLight(-1 * _camera.getPosition(), Vector<3>{1, 1, 1}, 0.7)), _ambiantLight(Vector<3>{1, 1, 1}, 0.2),
      _displayMode(REGULAR), _polygonMode(GL_FILL), _rotationSpeed(0.03), _translationSpeed(0.03), _zoomSpeed(0.1)
{
	_init();
}

Renderer &Renderer::operator=(const Renderer &other)
{
	if (this != &other)
	{
		_model = other._model;
		_shader = other._shader;
		_camera = other._camera;
		_polygonMode = other._polygonMode;
		_rotationSpeed = other._rotationSpeed;
		_zoomSpeed = other._zoomSpeed;
	}
	return *this;
}

void Renderer::render()
{
	const std::array<float, 3> &backgroundColor = _config.getBackground().color;
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_model->draw(_shader);
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

void Renderer::toggleAntialiasing()
{
	if (glIsEnabled(GL_MULTISAMPLE))
		glDisable(GL_MULTISAMPLE);
	else
		glEnable(GL_MULTISAMPLE);
}

void Renderer::changeDisplayMode()
{
	_displayMode = static_cast<DisplayMode>((_displayMode + 1) % 4);
	_shader.setUniform("displayMode", _displayMode);
}

Camera &Renderer::getCamera()
{
	return _camera;
}

Model &Renderer::getModel()
{
	return *_model;
}

Shader &Renderer::getShader()
{
	return _shader;
}

PointLight &Renderer::getLight()
{
	return _light;
}

float Renderer::getRotationSpeed() const
{
	return _rotationSpeed;
}

float Renderer::getTranslationSpeed() const
{
	return _translationSpeed;
}

float Renderer::getZoomSpeed() const
{
	return _zoomSpeed;
}

void Renderer::updateModel()
{
	Matrix<4, 4> model = _model->matrix();
	Matrix<4, 4> view = _camera.viewMatrix();
	Matrix<3, 3> normal = _normalMatrix(model, view);

	_shader.setUniform("model", model);
	_shader.setUniform("normal", normal);
}

void Renderer::updateLight()
{
	_shader.setUniform("lightPos", _light.getPosition());
}

void Renderer::resize(int width, int height)
{
	_camera.resize(width, height);
	_shader.setUniform("proj", _camera.projectionMatrix());
}

void Renderer::_init()
{
	// Set shader uniforms
	_shader.use();
	Matrix<4, 4> model = _model->matrix();
	Matrix<4, 4> view = _camera.viewMatrix();
	Matrix<3, 3> normal = _normalMatrix(model, view);

	_shader.setUniform("proj", _camera.projectionMatrix());
	_shader.setUniform("view", view);
	_shader.setUniform("model", model);
	_shader.setUniform("normal", normal);

	_shader.setUniform("lightColor", _light.getColor());
	_shader.setUniform("lightPos", _light.getPosition());
	_shader.setUniform("lightBrightness", _light.getBrightness());
	_shader.setUniform("ambiantLightColor", _ambiantLight.getColor());
	_shader.setUniform("ambiantLightBrightness", _ambiantLight.getBrightness());

	_shader.setUniform("cameraPos", _camera.getPosition());

	_shader.setUniform("diffuseColor", _model->getMaterial()->getDiffuse());
	_shader.setUniform("ambiantColor", _model->getMaterial()->getAmbient());
	_shader.setUniform("specularColor", _model->getMaterial()->getSpecular());
	_shader.setUniform("specularExponent", _model->getMaterial()->getSpecularExponent());

	if (_model->getMaterial()->hasTexture())
		_shader.setUniform("diffuseTexture", 0);
	if (_model->getMaterial()->hasNormalMap())
		_shader.setUniform("normalMap", 1);

	_shader.setUniform("displayMode", _displayMode);

	// OpenGL parameters
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);
}

Matrix<3, 3> Renderer::_normalMatrix(const Matrix<4, 4> &modelMatrix, const Matrix<4, 4> &viewMatrix)
{
	Matrix<4, 4> normal4 = viewMatrix.mul_mat(modelMatrix);
	normal4.inverse();
	normal4.transpose();
	Matrix<3, 3> normal = normal4.resize<3, 3>(false);
	return normal;
}
