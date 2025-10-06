#pragma once

#include "Config.hpp"
#include "MaterialLibrary.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glad/gl.h>

class Engine
{
  public:
	Engine();
	~Engine();

	void render();
	void loadModel(const std::string &filepath);
	void loadMaterialLibrary(const std::string &filepath);
	void updateActiveMaterial();
	void updateActiveMaterial(const std::string &name);

	static std::ifstream openFile(const std::string &filepath, const std::string &extension);

  private:
	Config      _config;
	GLFWwindow *_window;

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
