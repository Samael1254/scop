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

	enum Table : uint8_t
	{
		ROOT,
		WINDOW,
		BACKGROUND
	};

	void _loadConfig(const std::string &confFile);

	static Table _switchTable(std::string &line);

	void _loadRoot(const std::string &key, const std::string &value);
	void _loadWindow(const std::string &key, const std::string &value);
	void _loadBackground(const std::string &key, const std::string &value);

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
};
