#include "Camera.hpp"
#include "liblinal.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include <cmath>

Camera::Camera(int windowWidth, int windowHeight)
    : _fov(M_PI_2), _near(1), _far(100), _ratio(static_cast<float>(windowWidth) / static_cast<float>(windowHeight)),
      _position({0, 0, -5}), _direction({0, 0, -1}), _right(cross_product({0, 1, 0}, _direction)),
      _up(cross_product(_right, _direction))
{
}

Matrix<4, 4> Camera::projectionMatrix() const
{
	return ::projection(_fov, _ratio, _near, _far);
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
