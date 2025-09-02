#pragma once

#include "Vector.hpp"
#include <optional>
class Vertex
{
  public:
	Vertex();
	Vertex(const Vector<3> &position);
	Vertex(const Vector<3> &position, const Vector<2> &uv);
	Vertex(const Vector<3> &position, const Vector<3> &normal);
	Vertex(const Vector<3> &position, const Vector<3> &normal, const Vector<2> &uv);

	Vector<3> &position();
	Vector<3> &normal();
	Vector<2> &uv();

  private:
	Vector<3>                _position;
	std::optional<Vector<3>> _normal;
	std::optional<Vector<2>> _uv;
};
