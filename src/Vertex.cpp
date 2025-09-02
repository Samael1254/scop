#include "Vertex.hpp"
#include <stdexcept>

Vertex::Vertex() : _position({0, 0, 0}) {}

Vertex::Vertex(const Vector<3> &position) : _position(position) {}

Vertex::Vertex(const Vector<3> &position, const Vector<2> &uv) : _position(position), _uv(uv) {}

Vertex::Vertex(const Vector<3> &position, const Vector<3> &normal) : _position(position), _normal(normal) {}

Vertex::Vertex(const Vector<3> &position, const Vector<3> &normal, const Vector<2> &uv)
    : _position(position), _normal(normal), _uv(uv)
{
}

Vector<3> &Vertex::position()
{
	return _position;
}

Vector<3> &Vertex::normal()
{
	if (!_normal.has_value())
		throw std::logic_error("this vertex as no normal defined");
	return _normal.value();
}
Vector<2> &Vertex::uv()
{
	if (!_uv.has_value())
		throw std::logic_error("this vertex as no uv defined");
	return _uv.value();
}
