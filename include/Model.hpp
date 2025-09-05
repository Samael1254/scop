#pragma once

#include "Shader.hpp"
#include "Vector.hpp"
#include "VertexIndices.hpp"
#include "liblinal.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Model
{
  public:
	Model(const std::string &filepath);
	Model(const Model &other);

	Model &operator=(const Model &other);

	void draw(const Shader &shader);

	Matrix<4, 4> matrix() const;

	void scale(float scale);
	void incrementScale(float increment);
	void rotate(const Vector<3> &rotator);
	void incrementRotation(float angle, EAxis axis);
	void translate(const Vector<3> &translator);
	void incrementTranslation(float distance, EAxis axis);

  private:
	Vector<3> _position;
	Vector<3> _rotation;
	Vector<3> _scale;

	std::vector<Vector<3>> _vs;
	std::vector<Vector<3>> _vns;
	std::vector<Vector<2>> _vts;

	std::unordered_map<VertexIndices, uint64_t, std::hash<VertexIndices>> _indicesMap;

	std::vector<uint32_t> _elementBuffer;
	std::vector<float>    _vertexBuffer;

	uint32_t _vertexArrayID;
	uint32_t _vertexBufferID;
	uint32_t _elementBufferID;

	void             _loadModel(const std::string &filepath);
	static Vector<3> _readVector3(std::string &data);
	static Vector<2> _readVector2(std::string &data);
	void             _readFace(std::string &data);
	static void      _triangulateFace(std::vector<std::string> &lineData);
	VertexIndices    _readVertexIndices(const std::string &data);
	template <unsigned int N>
	int                _readVertexIndex(const std::string &data, const std::vector<Vector<N>> &vec);
	void               _createVertex(const VertexIndices &vi, Vector<3> normal);
	static std::string _getNextWord(std::string &line);
	static std::string _getNextWord(std::string &line, const std::string &separators);
	static Vector<3>   _computeNormal(const std::array<Vector<3>, 3> &vertices);

	void _center();
	void _setup();
};
