#pragma once

#include "Shader.hpp"
#include "Vector.hpp"
#include "liblinal.hpp"
#include <cstdint>
#include <string>
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
	void incerementScale(float increment);
	void rotate(const Vector<3> &rotator);
	void incrementRotation(float angle, EAxis axis);
	void translate(const Vector<3> &translator);
	void incrementTranslation(float distance, EAxis axis);
	void center();

  private:
	Vector<3> _position;
	Vector<3> _rotation;
	Vector<3> _scale;

	std::vector<Vector<3>>           _vs;
	std::vector<Vector<3>>           _vns;
	std::vector<Vector<2>>           _vts;
	std::vector<Vector<3, uint32_t>> _fs;

	std::vector<uint32_t> _indices;
	std::vector<float>    _vertices;

	uint32_t _vertexArrayID;
	uint32_t _vertexBufferID;
	uint32_t _elementBufferID;

	void                                    _loadModel(const std::string &filepath);
	static Vector<3>                        _readVector3(std::string &data);
	static Vector<2>                        _readVector2(std::string &data);
	void                                    _readFace(std::string &data);
	static std::string                      _getNextWord(std::string &line);
	static std::vector<Vector<3, uint32_t>> _triangulateFace(const std::vector<uint32_t> &indices);

	void                  _setup();
	std::vector<uint32_t> _createIndices();
	std::vector<float>    _createVertices();
};
