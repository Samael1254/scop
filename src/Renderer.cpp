#include "Renderer.hpp"
#include "liblinal.hpp"
#include "Shader.hpp"
#include <cmath>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

Renderer::Renderer(int width, int height)
    : _model(Model("resources/42.obj")), _shader(Shader("vertexShader.vert", "fragmentShader.frag")),
      _camera(width, height), _polygonMode(GL_FILL)
{
	_init();
}

void Renderer::render()
{
	glClearColor(0.102, 0.188, 0.29, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_model.draw(_shader);
	// for (uint32_t i = 0; i < _vertexArrayIDs.size(); ++i)
	// {
	// double timeValue = glfwGetTime();
	// _shaders[i].use();
	// _shaders[i].setUniform("rot", Matrix<4, 4>::createRotationMatrix(std::sin(timeValue) * 2 * M_PI, ZAxis));
	// glBindVertexArray(_vertexArrayIDs[i]);
	// glDrawArrays(GL_TRIANGLES, 0, 3);
	// }
}

void Renderer::setPolygonMode(int polygonMode)
{
	if (polygonMode != GL_FILL || polygonMode != GL_LINE || polygonMode != GL_POINT)
		throw std::logic_error("invalid polygon mode");
	_polygonMode = polygonMode;
}

void Renderer::_init()
{
	_shader.use();
	_shader.setUniform("proj", _camera.projectionMatrix());
	_shader.setUniform("view", _camera.viewMatrix());
	_shader.setUniform("model", _model.matrix());
	// clang-format off
	// float vertices[] = {
	//     +0.5F, -0.5F, 0.0F,
	// 	-0.5F, -0.5F, 0.0F,
	// 	+0.0F, +0.5F, 0.0F,
	// };
	// uint32_t indices[] = {
	//     0, 1, 3, 1, 2, 3,
	// };
	// clang-format on

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);
}
