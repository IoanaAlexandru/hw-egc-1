#include "Brick.h"

Brick::Brick(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill)
	: Rectangle(name, topLeftCorner, height, width, color, fill)
{
}


Brick::~Brick()
{
}
