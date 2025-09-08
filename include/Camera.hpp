#include "Matrix.hpp"
#include "Vector.hpp"

class Camera
{
  public:
	Camera(int windowWidth, int windowHeight);
	Camera(const Camera &other) = default;

	Camera &operator=(const Camera &other);

	void resize(int width, int height);

	Matrix<4, 4> projectionMatrix() const;
	Matrix<4, 4> viewMatrix() const;

	const Vector<3> &getPosition() const;

  private:
	float _fov;
	float _near;
	float _far;
	float _ratio;

	Vector<3> _position;
	Vector<3> _direction;
	Vector<3> _right;
	Vector<3> _up;
};
