#pragma once

#include "Model.hpp"
#include "Vector.hpp"
#include "VertexIndices.hpp"
#include <unordered_map>
#include <vector>

class ObjParser
{
  public:
	Model loadModel(const std::string &filepath);

  private:
	std::vector<Vector<3>> _vs;
	std::vector<Vector<3>> _vns;
	std::vector<Vector<2>> _vts;

	std::unordered_map<VertexIndices, uint64_t, std::hash<VertexIndices>> _indicesMap;

	std::vector<uint32_t> _elementBuffer;
	std::vector<float>    _vertexBuffer;

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
};
