#pragma once

#include "Material.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include <cstdint>
#include <string>
#include <vector>
class Mesh
{
  public:
	Mesh() = default;
	Mesh(const std::string &name);
	~Mesh() = default;

	void draw(const Shader &shader);

  private:
	std::string           _name;
	std::vector<Vertex>   _vertices;
	std::vector<uint32_t> _elements;

	uint32_t _vertexArrayID;
	uint32_t _vertexBufferID;
	uint32_t _elementBufferID;

	Material _material;

	void _init();
};
