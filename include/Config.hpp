#pragma once

#include <array>
#include <string>
#include <sys/types.h>

class Config
{
  public:
	Config();
	void print() const;

  private:
	struct Root
	{
		bool verbose = false;
		bool antialiasing = false;
	} _root;
	struct Window
	{
		int  width = 800;
		int  height = 600;
		bool fullscreen = false;
	} _window;
	struct Background
	{
		std::array<float, 3> color = {0.384, 0.474, 0.549};
	} _background;
	struct Camera
	{
		float fov = 1.047;
	} _camera;
	struct Object
	{
		float rotationSpeed = 0.03;
		float translationSpeed = 0.03;
		float zoomSpeed = 0.1;
	} _object;
	struct Light
	{
		std::array<float, 3> color = {1, 1, 1};
		float                brightness = 0.7;
	} _light;
	struct Ambiant
	{
		std::array<float, 3> color = {1, 1, 1};
		float                brightness = 0.2;
	} _ambiant;

	enum Table : uint8_t
	{
		ROOT,
		WINDOW,
		BACKGROUND,
		CAMERA,
		OBJECT,
		LIGHT,
		AMBIANT,
	};

	void _loadConfig(const std::string &confFile);

	static Table _switchTable(std::string &line);

	void _loadRoot(const std::string &key, const std::string &value);
	void _loadWindow(const std::string &key, const std::string &value);
	void _loadBackground(const std::string &key, const std::string &value);
	void _loadCamera(const std::string &key, const std::string &value);
	void _loadObject(const std::string &key, const std::string &value);
	void _loadLight(const std::string &key, const std::string &value);
	void _loadAmbiant(const std::string &key, const std::string &value);

	std::string _parseKey(const std::string &line);
	std::string _parseValue(const std::string &line);

	bool                   _readBool(const std::string &value);
	uint32_t               _readUInt(const std::string &value);
	std::string            _readString(const std::string &value);
	std::array<uint8_t, 3> _readColor(const std::string &value);

  public:
	const Root       &getRoot() const;
	const Window     &getWindow() const;
	const Background &getBackground() const;
	const Camera     &getCamera() const;
	const Object     &getObject() const;
	const Light      &getLight() const;
	const Ambiant    &getAmbiant() const;
};
