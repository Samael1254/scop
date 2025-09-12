#pragma once

#include "MaterialLibrary.hpp"
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
	void loadModel(const std::string &filepath);
	void loadMaterialLibrary(const std::string &filepath);
	void updateActiveMaterial();
	void updateActiveMaterial(const std::string &name);

  private:
	int             _width;
	int             _height;
	GLFWwindow     *_window;
	bool            _verbose;
	Model           _model;
	MaterialLibrary _mtl;

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
