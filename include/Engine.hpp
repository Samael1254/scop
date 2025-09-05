#pragma once

#include "Model.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <glad/gl.h>

class Engine
{
  public:
	Engine(bool verbose = false);
	Engine(int width, int height, bool verbose = false);
	~Engine();

	void render(Model &model);

  private:
	int         _width;
	int         _height;
	GLFWwindow *_window;
	bool        _verbose;
	// Renderer    _renderer;

	void        _init();
	void        _initGLFW() const;
	void        _createWindow();
	static void _frameBufferSizeCallback(GLFWwindow *window, int width, int height);
	void        _initGLAD();

	void _close();

	void        _renderLoop(Model &model);
	void        _processInput(Renderer &renderer);
	static void _mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
	static void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
