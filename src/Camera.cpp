#include "Camera.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "liblinal.hpp"
#include <cmath>

Camera::Camera(int windowWidth, int windowHeight)
    : _fov(M_PI / 3), _near(1.0F), _far(100.0F),
      _ratio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)), _position({0, 0, -5}),
      _direction({0, 0, -1}), _right(cross_product({0, 1, 0}, _direction)), _up(cross_product(_direction, _right))
{
}

Camera &Camera::operator=(const Camera &other)
{
	if (this != &other)
	{
		_fov = other._fov;
		_near = other._near;
		_far = other._far;
		_ratio = other._ratio;
		_position = other._position;
		_direction = other._direction;
		_right = other._right;
		_up = other._up;
	}
	return *this;
}
Matrix<4, 4> Camera::projectionMatrix() const
{
	Matrix<4, 4> proj = ::projection(_fov, _ratio, _near, _far);
	return proj;
}

Matrix<4, 4> Camera::viewMatrix() const
{
	Matrix<4, 4> view;
	Vector<3>    vectors[] = {_right, _up, _direction, _position};

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 3; ++j)
			view[j][i] = vectors[i][j];
	view[3][3] = 1;

	return view;
}

const Vector<3> &Camera::getPosition() const
{
	return _position;
}
