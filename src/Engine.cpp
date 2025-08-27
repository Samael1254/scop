#include "Engine.hpp"
#include <GLFW/glfw3.h>
#include <stdexcept>

Engine::Engine(int width, int height) : _width(width), _height(height), _window(nullptr)
{
	_init();
}

Engine::Engine() : _width(1000), _height(800), _window(nullptr)
{
	_init();
}

Engine::~Engine()
{
	if (_window)
		glfwDestroyWindow(_window);
	glfwTerminate();
}

void Engine::run()
{
	_renderLoop();
}

void Engine::_init()
{
	_initGLFW();
	_createWindow();
	_initGLAD();
}

void Engine::_initGLFW()
{
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("failed to initialize GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Engine::_createWindow()
{
	_window = glfwCreateWindow(_width, _height, "Scop", nullptr, nullptr);
	if (!_window)
	{
		glfwTerminate();
		throw std::runtime_error("failed to create window");
	}
	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, _frameBufferSizeCallback);
}

void Engine::_initGLAD() const
{
	int gladVersion = gladLoadGL(glfwGetProcAddress);
	if (gladVersion == 0)
		throw std::runtime_error("failed to initialize OpenGL context through GLAD");
	glViewport(0, 0, _width, _height);
}

void Engine::_renderLoop()
{
	while (!glfwWindowShouldClose(_window))
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Engine::_frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}
