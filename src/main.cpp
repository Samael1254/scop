#include "Engine.hpp"
#include <exception>
#include <iostream>

int main(void)
{
	try
	{
		Engine engine(1000, 800, true);
		engine.run();
	}
	catch (std::exception &e)
	{
		std::cerr << "\e[31mError:\e[0m " << e.what() << std::endl;
		return 1;
	}

	std::cout << "Exit program\n";
	return 0;
}
