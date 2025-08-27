#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Engine
{
  public:
	Engine();
	Engine(int width, int height);
	~Engine();

	void run();

  private:
	int         _width;
	int         _height;
	GLFWwindow *_window;

	void        _init();
	static void _initGLFW();
	void        _createWindow();
	void        _initGLAD() const;
	void        _renderLoop();

	static void _frameBufferSizeCallback(GLFWwindow *window, int width, int height);
};
