#include "Vector.hpp"
class Light
{
  public:
	Light();

  private:
	Vector<4> _color;
	float     _brightness;
	Vector<3> _position;
};
