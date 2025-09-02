#include "Model.hpp"
#include <fstream>
#include <string>
#include <sys/types.h>
#include <vector>

Model::Model(const std::string &filepath)
{
	_loadModel(filepath);
}

void Model::_loadModel(const std::string &filepath) {}

void Model::_loadMesh(const std::string &name) {}

void Model::_readVertices(const std::string &filepath)
{
	std::ifstream is(filepath);
	if (!is.is_open())
		throw std::runtime_error("failed to open .obj file: " + filepath);

	std::string buf;
	while (std::getline(is, buf))
	{
		u_int32_t   end = buf.find(' ');
		std::string type = buf.substr(0, end);
		if (type == "v")
		{
		}
		else if (type == "vn")
		{
		}
		else if (type == "vt")
		{
		}
	}
}
