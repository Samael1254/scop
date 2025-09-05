#include "Vector.hpp"
class Light
{
  public:
	Light();
	Light(const Vector<3> &position);

	Vector<3> getColor() const;
	float     getBrightness() const;
	Vector<3> getPosition() const;

  private:
	Vector<3> _color;
	float     _brightness;
	Vector<3> _position;
};
