#include "Model.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include "liblinal.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>

Model::Model(const std::string &filepath) : _scale({1, 1, 1})
{
	_loadModel(filepath);
	_setup();
	// for (unsigned int i = 0; i < _vertices.size(); i++)
	// 	std::cout << _vertices[i] << " ";
	// std::cout << "\n ";
	// for (unsigned int i = 0; i < _indices.size(); i++)
	// 	std::cout << _indices[i] << " ";
	// std::cout << "\n ";
}

Model::Model(const Model &other)
{
	*this = other;
}

Model &Model::operator=(const Model &other)
{
	if (this != &other)
	{
		_scale = other._scale;
		_position = other._position;
		_rotation = other._rotation;
		_vs = other._vs;
		_vns = other._vns;
		_vts = other._vts;
		_fs = other._fs;
		_indices = other._indices;
		_vertices = other._vertices;
		_vertexArrayID = other._vertexArrayID;
		_vertexBufferID = other._vertexBufferID;
		_elementBufferID = other._elementBufferID;
	}
	return *this;
}

void Model::draw(const Shader &shader)
{
	shader.use();
	glBindVertexArray(_vertexArrayID);
	glDrawElements(GL_TRIANGLES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Matrix<4, 4> Model::matrix() const
{
	Matrix<4, 4> model;
	model = Matrix<4, 4>::createTranslationMatrix(_position);
	for (unsigned int i = 0; i < 3; ++i)
		if (_rotation[i] != 0)
			model = model * Matrix<4, 4>::createRotationMatrix(_rotation[i], static_cast<EAxis>(i));
	model = model * Matrix<4, 4>::createScalingMatrix({_scale.x(), _scale.y(), _scale.z(), 1});
	return model;
}

void Model::scale(float scale)
{
	_scale = {scale, scale, scale};
}

void Model::incerementScale(float increment)
{
	_scale *= increment;
	center();
}

void Model::rotate(const Vector<3> &rotator)
{
	_rotation = rotator;
}

void Model::incrementRotation(float angle, EAxis axis)
{
	_rotation[axis] += angle;
}

void Model::translate(const Vector<3> &translator)
{
	_position = translator;
}

void Model::incrementTranslation(float distance, EAxis axis)
{
	_position[axis] += distance;
}

void Model::center()
{
	if (_vs.size() == 0)
		return;

	Vector<3> max = Matrix<3, 3>::createScalingMatrix(_scale) * _vs[0];
	Vector<3> min = max;
	for (unsigned int i = 1; i < _vs.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			max[j] = std::max(_vs[i][j] * _scale[j], max[j]);
			min[j] = std::min(_vs[i][j] * _scale[j], min[j]);
		}
	}

	Vector<3> center = (min + max) * 0.5;
	_position = -1 * center;
}

void Model::_loadModel(const std::string &filepath)
{
	std::ifstream is(filepath);
	if (!is.is_open())
		throw std::runtime_error("failed to open .obj file: " + filepath);

	std::string              buf;
	std::vector<std::string> validTypes = {"v", "vs", "vt", "f"};
	while (std::getline(is, buf))
	{
		std::string type = _getNextWord(buf);
		if (std::find(validTypes.begin(), validTypes.end(), type) == validTypes.end())
			continue;

		if (type == "v")
			_vs.push_back(_readVector3(buf));
		else if (type == "vn")
			_vns.push_back(_readVector3(buf));
		else if (type == "vt")
			_vts.push_back(_readVector2(buf));
		else if (type == "f")
			_readFace(buf);
	}
	// for (unsigned int i = 0; i < _fs.size(); i++)
	// 	std::cout << _fs[i][0] << " " << _fs[i][1] << " " << _fs[i][2] << "\n";
}

std::string Model::_getNextWord(std::string &line)
{
	unsigned long end = line.find(' ');
	if (end == std::string::npos)
	{
		std::string word = line;
		line = "";
		return word;
	}
	std::string word = line.substr(0, end);
	line = line.substr(line.find_first_not_of(" \t", end), line.length() - end);
	return word;
}

Vector<3> Model::_readVector3(std::string &data)
{
	Vector<3> vector;

	for (int i = 0; i < 3; ++i)
		vector[i] = static_cast<float>(std::atof(_getNextWord(data).c_str()));
	return vector;
}

Vector<2> Model::_readVector2(std::string &data)
{
	Vector<2> vector;

	for (int i = 0; i < 2; ++i)
		vector[i] = static_cast<float>(std::atof(_getNextWord(data).c_str()));
	return vector;
}

void Model::_readFace(std::string &data)
{
	std::vector<uint32_t> indices;

	while (!data.empty())
		indices.push_back(std::atoi(_getNextWord(data).c_str()));
	if (indices.size() == 3)
	{
		Vector<3, uint32_t> face;
		for (int i = 0; i < 3; ++i)
			face[i] = indices[i];
		_fs.push_back(face);
		return;
	}
	std::vector<Vector<3, uint32_t>> faces = _triangulateFace(indices);
	_fs.insert(_fs.end(), faces.begin(), faces.end());
}

std::vector<Vector<3, uint32_t>> Model::_triangulateFace(const std::vector<uint32_t> &indices)
{
	std::vector<Vector<3, uint32_t>> faces;
	for (uint32_t i = 0; i < indices.size() - 2; ++i)
	{
		Vector<3, uint32_t> face;
		face[0] = indices[0];
		face[1] = indices[i + 1];
		face[2] = indices[i + 2];
		faces.push_back(face);
	}
	return faces;
}

void Model::_setup()
{
	glGenVertexArrays(1, &_vertexArrayID);
	glGenBuffers(1, &_vertexBufferID);
	glGenBuffers(1, &_elementBufferID);

	glBindVertexArray(_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);

	_indices = _createIndices();
	_vertices = _createVertices();

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _indices.size(), _indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	// glEnableVertexAttribArray(1);
}

std::vector<uint32_t> Model::_createIndices()
{
	std::vector<uint32_t> indices;
	for (uint32_t i = 0; i < _fs.size(); ++i)
		for (uint32_t j = 0; j < 3; ++j)
			indices.push_back(_fs[i][j] - 1);
	return indices;
}

std::vector<float> Model::_createVertices()
{
	std::vector<float> vertices;
	for (uint32_t i = 0; i < _vs.size(); ++i)
		for (uint32_t j = 0; j < 3; ++j)
			vertices.push_back(_vs[i][j]);
	return vertices;
}
