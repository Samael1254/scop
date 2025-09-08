#include "Model.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Vector.hpp"
#include "liblinal.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include <vector>

Model::Model(const std::string &filepath) : _scale({1, 1, 1})
{
	_loadModel(filepath);
	_init();
	_setup();
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
		_material = other._material;
		_vs = other._vs;
		_vns = other._vns;
		_vts = other._vts;
		_elementBuffer = other._elementBuffer;
		_vertexBuffer = other._vertexBuffer;
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
	glDrawElements(GL_TRIANGLES, static_cast<int>(_elementBuffer.size()), GL_UNSIGNED_INT, 0);
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

// Matrix<3, 3> Model::normalMatrix() const
// {
// 	Matrix<4, 4> model;
// 	model = Matrix<4, 4>::createTranslationMatrix(_position);
// 	for (unsigned int i = 0; i < 3; ++i)
// 		if (_rotation[i] != 0)
// 			model = model * Matrix<4, 4>::createRotationMatrix(_rotation[i], static_cast<EAxis>(i));
// 	model = model * Matrix<4, 4>::createScalingMatrix({_scale.x(), _scale.y(), _scale.z(), 1});
// 	return model;
// }

void Model::scale(float scale)
{
	_scale = {scale, scale, scale};
}

void Model::incrementScale(float increment)
{
	_scale *= increment;
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

const Material &Model::getMaterial() const
{
	return _material;
}

void Model::setMaterial(Material &material)
{
	_material = material;
}

void Model::_init()
{
	if (_vs.empty())
		return;

	// Find min and max bounds of models
	Vector<3> max = _vs[0];
	Vector<3> min = max;
	for (unsigned int i = 1; i < _vs.size(); ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			max[j] = std::max(_vs[i][j], max[j]);
			min[j] = std::min(_vs[i][j], min[j]);
		}
	}

	// Initialize scale
	Vector<3> span = max - min;
	float     maxSpan = std::max(std::max(span.x(), span.y()), span.z());
	scale(2.5F / maxSpan);

	// Center
	Vector<3> center = (min + max) * 0.5;
	for (unsigned int i = 0; i < _vertexBuffer.size(); i += 8)
	{
		_vertexBuffer[i] -= center[0];
		_vertexBuffer[i + 1] -= center[1];
		_vertexBuffer[i + 2] -= center[2];
	}
}

void Model::_loadModel(const std::string &filepath)
{
	std::ifstream is(filepath);
	if (!is.is_open())
		throw std::runtime_error("failed to open .obj file: " + filepath);

	std::string              buf;
	std::vector<std::string> validTypes = {"v", "vt", "vn", "f"};
	while (std::getline(is, buf))
	{
		std::string type = _getNextWord(buf);
		if (std::find(validTypes.begin(), validTypes.end(), type) == validTypes.end())
			continue;

		if (type == "v")
			_vs.push_back(_readVector3(buf));
		else if (type == "vt")
			_vts.push_back(_readVector2(buf));
		else if (type == "vn")
			_vns.push_back(_readVector3(buf));
		else if (type == "f")
			_readFace(buf);
	}
}

std::string Model::_getNextWord(std::string &line)
{
	return _getNextWord(line, " \t");
}

std::string Model::_getNextWord(std::string &line, const std::string &separators)
{
	unsigned long end = line.find_first_of(separators);
	if (end == std::string::npos)
	{
		std::string word = line;
		line = "";
		return word;
	}
	std::string word = line.substr(0, end);
	line = line.substr(line.find_first_not_of(separators, end), line.length() - end);
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
	std::vector<std::array<int32_t, 3>> indices;

	std::vector<std::string> lineData;
	while (!data.empty())
		lineData.push_back(_getNextWord(data));
	if (lineData.size() < 3)
		throw std::runtime_error("face has less than three vertices in line:\n" + data);
	if (lineData.size() > 3)
		_triangulateFace(lineData);
	for (unsigned int i = 0; i < lineData.size() / 3; ++i)
	{
		std::array<VertexIndices, 3> vis;
		std::array<Vector<3>, 3>     positions;
		for (unsigned int j = 0; j < 3; ++j)
		{
			vis[j] = _readVertexIndices(lineData[3 * i + j]);
			positions[j] = _vs[vis[j].positionID];
		}
		Vector<3> faceNormals;
		bool      hasNormal = vis[0].normalID != -1 && vis[1].normalID != -1 && vis[2].normalID != -1;
		if (!hasNormal)
			faceNormals = _computeNormal(positions);
		for (unsigned int j = 0; j < 3; ++j)
		{
			std::unordered_map<VertexIndices, uint64_t, std::hash<VertexIndices>>::iterator it =
			    _indicesMap.find(vis[j]);
			if (it != _indicesMap.end())
			{
				_elementBuffer.push_back(it->second);
				continue;
			}
			uint32_t index = static_cast<uint32_t>(_indicesMap.size());
			_indicesMap.emplace(vis[j], index);
			_elementBuffer.push_back(index);
			if (!hasNormal)
				_createVertex(vis[j], faceNormals);
			else
				_createVertex(vis[j], _vns[vis[j].normalID]);
		}
	}
}

void Model::_triangulateFace(std::vector<std::string> &lineData)
{
	std::vector<std::string> trLineData;
	for (uint32_t i = 0; i < lineData.size() - 2; ++i)
	{
		trLineData.push_back(lineData[0]);
		trLineData.push_back(lineData[i + 1]);
		trLineData.push_back(lineData[i + 2]);
	}
	lineData = trLineData;
}

VertexIndices Model::_readVertexIndices(const std::string &data)
{
	VertexIndices vi(_readVertexIndex(data, _vs));

	unsigned long firstSlash = data.find('/');
	if (firstSlash == std::string::npos)
		return vi;
	unsigned long secondSlash = data.find('/', firstSlash + 1);
	if (secondSlash > firstSlash + 1)
		vi.textureID = _readVertexIndex(data.substr(firstSlash + 1), _vts);
	if (secondSlash == std::string::npos)
		return vi;
	vi.normalID = _readVertexIndex(data.substr(secondSlash + 1), _vns);
	return vi;
}

template <unsigned int N>
int Model::_readVertexIndex(const std::string &data, const std::vector<Vector<N>> &vec)
{
	int index = std::atoi(data.c_str());
	if (index < 0)
		index = static_cast<int>(vec.size()) + index;
	else
		index -= 1;

	if (index < 0 || index >= static_cast<int>(vec.size()))
		throw std::out_of_range("OBJ index out of range");

	return index;
}

void Model::_createVertex(const VertexIndices &vi, Vector<3> normal)
{
	for (unsigned int i = 0; i < 3; ++i)
		_vertexBuffer.push_back(_vs[vi.positionID][i]);
	if (vi.textureID == -1)
	{
		_vertexBuffer.push_back(0.0F);
		_vertexBuffer.push_back(0.0F);
	}
	else
		for (unsigned int i = 0; i < 2; ++i)
			_vertexBuffer.push_back(_vts[vi.textureID][i]);
	for (unsigned int i = 0; i < 3; ++i)
		_vertexBuffer.push_back(normal[i]);
}

Vector<3> Model::_computeNormal(const std::array<Vector<3>, 3> &vertices)
{
	Vector<3> normal;
	Vector<3> ab = vertices[1] - vertices[0];
	Vector<3> bc = vertices[2] - vertices[1];
	normal = cross_product(ab, bc).normalized();

	return normal;
}

void Model::_setup()
{
	glGenVertexArrays(1, &_vertexArrayID);
	glGenBuffers(1, &_vertexBufferID);
	glGenBuffers(1, &_elementBufferID);

	glBindVertexArray(_vertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _vertexBuffer.size(), _vertexBuffer.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * _elementBuffer.size(), _elementBuffer.data(),
	             GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
