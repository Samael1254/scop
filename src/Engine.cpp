#include "Engine.hpp"
#include "ImageSaver.hpp"
#include "MaterialLibrary.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "liblinal.hpp"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

Engine::Engine(bool verbose) : _width(1000), _height(800), _window(nullptr), _verbose(verbose)
{
	_init();
}

Engine::Engine(int width, int height, bool verbose)
    : _width(width), _height(height), _window(nullptr), _verbose(verbose)
{
	_init();
}

Engine::~Engine()
{
	_close();
}

void Engine::render()
{
	_renderLoop();
}

void Engine::loadModel(const std::string &filepath)
{
	// _model = Model(filepath, _mtl.getLastMaterial());
	_model = Model(filepath, _mtl);
}

void Engine::loadMaterialLibrary(const std::string &filepath)
{
	_mtl = MaterialLibrary(filepath);
}

void Engine::updateActiveMaterial()
{
	_model.setMaterial(_mtl.getLastMaterial());
}

void Engine::updateActiveMaterial(const std::string &name)
{
	_model.setMaterial(_mtl.getMaterial(name));
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
	glfwWindowHint(GLFW_SAMPLES, 4);
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
	glfwSetScrollCallback(_window, &Engine::_mouseScrollCallback);
	glfwSetKeyCallback(_window, _keyCallback);
}

void Engine::_frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	Renderer *rendererAddr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	rendererAddr->resize(width, height);
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
	Renderer renderer(_width, _height, &_model);
	glfwSetWindowUserPointer(_window, &renderer);
	glfwSetFramebufferSizeCallback(_window, _frameBufferSizeCallback);

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
	{
		glfwSetWindowShouldClose(_window, true);
		if (_verbose)
			std::cout << "Exit program\n";
	}

	// Object rotation
	if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		renderer.getModel().rotate(renderer.getRotationSpeed(), YAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		renderer.getModel().rotate(-renderer.getRotationSpeed(), YAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		renderer.getModel().rotate(-renderer.getRotationSpeed(), XAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		renderer.getModel().rotate(renderer.getRotationSpeed(), XAxis);
		renderer.updateModel();
	}
	// Object translation
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		renderer.getModel().translate(renderer.getTranslationSpeed(), XAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		renderer.getModel().translate(-renderer.getTranslationSpeed(), XAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		renderer.getModel().translate(renderer.getTranslationSpeed(), YAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		renderer.getModel().translate(-renderer.getTranslationSpeed(), YAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		renderer.getModel().translate(renderer.getTranslationSpeed(), ZAxis);
		renderer.updateModel();
	}
	if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		renderer.getModel().translate(-renderer.getTranslationSpeed(), ZAxis);
		renderer.updateModel();
	}
	// Light translation
	if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS)
	{
		renderer.getLight().rotate(renderer.getRotationSpeed(), YAxis);
		renderer.updateLight();
	}
	if (glfwGetKey(_window, GLFW_KEY_H) == GLFW_PRESS)
	{
		renderer.getLight().rotate(-renderer.getRotationSpeed(), YAxis);
		renderer.updateLight();
	}
	if (glfwGetKey(_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		renderer.getLight().rotate(-renderer.getRotationSpeed(), XAxis);
		renderer.updateLight();
	}
	if (glfwGetKey(_window, GLFW_KEY_J) == GLFW_PRESS)
	{
		renderer.getLight().rotate(renderer.getRotationSpeed(), XAxis);
		renderer.updateLight();
	}
}

void Engine::_mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	(void)xOffset;
	Renderer *rendererAddr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	float     speed = yOffset == 1 ? 1 + rendererAddr->getZoomSpeed() : 1 - rendererAddr->getZoomSpeed();
	rendererAddr->getModel().scale(speed);
	rendererAddr->updateModel();
}

void Engine::_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	Renderer *rendererAddr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		rendererAddr->switchPolygonMode();
	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		Renderer::toggleAntialiasing();
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		rendererAddr->changeDisplayMode();
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		int width;
		int height;
		glfwGetWindowSize(window, &width, &height);
		ImageSaver::saveFrame("screenshots", width, height);
	}
}
