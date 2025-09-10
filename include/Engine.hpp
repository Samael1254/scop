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

	void render();
	void loadTexture(const std::string &filepath);
	void loadModel(const std::string &filepath);
	// void loadMaterial(const std::string &filepath);

  private:
	int         _width;
	int         _height;
	GLFWwindow *_window;
	bool        _verbose;
	Model       _model;
	Material    _material;
	Texture     _texture;

	void        _init();
	void        _initGLFW() const;
	void        _createWindow();
	static void _frameBufferSizeCallback(GLFWwindow *window, int width, int height);
	void        _initGLAD();

	void _close();

	void        _renderLoop();
	void        _processInput(Renderer &renderer);
	static void _mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
	static void _keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};
