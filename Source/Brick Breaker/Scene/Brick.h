#pragma once
#include "Brick Breaker/Generic/Rectangle.h"
class Brick :
	public Rectangle
{
public:
	// Create rectangle (brick) with given top left corner, size and color
	Brick(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill);
	~Brick();
};
