#include "Renderer.hpp"
#include "liblinal.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include <cmath>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>

Renderer::Renderer() : _polygonMode(GL_FILL)
{
	_init();
}

void Renderer::render()
{
	glClearColor(0.102, 0.188, 0.29, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (uint32_t i = 0; i < _vertexArrayIDs.size(); ++i)
	{
		double timeValue = glfwGetTime();
		_shaders[i].use();
		_shaders[i].setUniform("rot", Matrix<4, 4>::createRotationMatrix(std::sin(timeValue) * 2 * M_PI, ZAxis));
		glBindVertexArray(_vertexArrayIDs[i]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

void Renderer::setPolygonMode(int polygonMode)
{
	if (polygonMode != GL_FILL || polygonMode != GL_LINE || polygonMode != GL_POINT)
		throw std::logic_error("invalid polygon mode");
	_polygonMode = polygonMode;
}

void Renderer::_init()
{
	_shaders.push_back(Shader("vertexShader.vert", "fragmentShader.frag"));

	// clang-format off
	float vertices[] = {
	    +0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,
		-0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,
		+0.0F, +0.5F, 0.0F, 0.0F, 0.0F, 1.0F,
	};
	// clang-format on

	// uint32_t indices[] = {
	//     0, 1, 3, 1, 2, 3,
	// };

	uint32_t vao1;
	uint32_t vbo1;
	// uint32_t ebo;

	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	// glGenBuffers(1, &ebo);

	glBindVertexArray(vao1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	_vertexArrayIDs.push_back(vao1);
	// _elementBufferIDs.push_back(ebo);

	glPolygonMode(GL_FRONT_AND_BACK, _polygonMode);
}
