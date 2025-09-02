#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Engine
{
  public:
	Engine(bool verbose = false);
	Engine(int width, int height, bool verbose = false);
	~Engine();

	void run();

  private:
	int         _width;
	int         _height;
	GLFWwindow *_window;
	bool        _verbose;

	void        _init();
	void        _initGLFW() const;
	void        _createWindow();
	static void _frameBufferSizeCallback(GLFWwindow *window, int width, int height);
	void        _initGLAD();

	void _close();

	void _renderLoop();
	void _processInput();
};
