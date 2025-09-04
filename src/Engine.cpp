#include "Engine.hpp"
#include "Renderer.hpp"
#include "liblinal.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

Engine::Engine(bool verbose) : _width(1000), _height(800), _window(nullptr), _verbose(verbose) //_renderer(1000, 800),
{
	_init();
}
Engine::Engine(int width, int height, bool verbose)
    : _width(width), _height(height), _window(nullptr), _verbose(verbose) //_renderer(width, height),
{
	_init();
}

Engine::~Engine()
{
	_close();
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

void Engine::_initGLFW() const
{
	if (glfwInit() == GLFW_FALSE)
		throw std::runtime_error("failed to initialize GLFW");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (_verbose)
		std::cout << "Loaded GLFW" << "\n";
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
	glfwSetScrollCallback(_window, &Engine::_mouseScrollCallback);
	glfwSetKeyCallback(_window, _keyCallback);
}

void Engine::_frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	(void)window;
	glViewport(0, 0, width, height);
}

void Engine::_initGLAD()
{
	int gladVersion = gladLoadGL(glfwGetProcAddress);
	if (gladVersion == 0)
	{
		_close();
		throw std::runtime_error("failed to initialize OpenGL context through GLAD");
	}
	if (_verbose)
		std::cout << "Loaded OpenGL version " << GLAD_VERSION_MAJOR(gladVersion) << "."
		          << GLAD_VERSION_MINOR(gladVersion) << "\n";
	glViewport(0, 0, _width, _height);
}

void Engine::_close()
{
	if (_window)
		glfwDestroyWindow(_window);
	glfwTerminate();
	if (_verbose)
		std::cout << "Terminate GLFW and OpenGL\n";
}

void Engine::_renderLoop()
{
	// _renderer.init();
	Renderer renderer(_width, _height);
	glfwSetWindowUserPointer(_window, &renderer);

	while (!glfwWindowShouldClose(_window))
	{
		// Input
		_processInput(renderer);

		// Rendering
		try
		{
			renderer.render();
		}
		catch (std::exception &e)
		{
			throw std::runtime_error("render error: " + std::string(e.what()));
		}

		// Call events and swap buffers
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void Engine::_processInput(Renderer &renderer)
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
	if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		renderer.getModel().incrementRotation(renderer.getRotationSpeed(), YAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		renderer.getModel().incrementRotation(-renderer.getRotationSpeed(), YAxis);
		renderer.updateModel();
	}
}

void Engine::_mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	(void)xOffset;
	Renderer *rendererAddr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	float     speed = yOffset == 1 ? 1 + rendererAddr->getZoomSpeed() : 1 - rendererAddr->getZoomSpeed();
	rendererAddr->getModel().incerementScale(speed);
	rendererAddr->updateModel();
}

void Engine::_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	Renderer *rendererAddr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		rendererAddr->switchPolygonMode();
}
