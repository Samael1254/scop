#include "Engine.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv)
{
	try
	{
		if (argc == 1)
			throw std::invalid_argument("no argument: mesh file needed");
		if (argc > 3)
			throw std::invalid_argument("too many arguments");

		Engine engine(1000, 800);
		if (argc == 3)
			engine.loadTexture(argv[2]);
		engine.loadModel(argv[1]);
		engine.render();
	}
	catch (std::exception &e)
	{
		std::cerr << "\e[31mError:\e[0m " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
