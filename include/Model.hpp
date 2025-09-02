#pragma once

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include <string>
#include <vector>

class Model
{
  public:
	Model(const std::string &filepath);

	void draw(const Shader &shader);

  private:
	Vector<3> _position;
	Vector<3> _rotation;
	Vector<3> _scale;

	std::vector<Vector<3>> _vs;
	std::vector<Vector<3>> _vns;
	std::vector<Vector<2>> _vts;

	std::vector<Mesh> _meshes;

	void _loadModel(const std::string &filepath);
	void _loadMesh(const std::string &name);
	void _readVertices(const std::string &filepath);
};
