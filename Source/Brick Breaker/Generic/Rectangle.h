#pragma once
#include "AnimatedMesh.h"
class Rectangle :
	public AnimatedMesh
{
public:
	// Create rectangle with given top left corner, size, color and fill
	Rectangle(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill);
	~Rectangle();

protected:
	float height_,
		width_;
	glm::vec3 color_;
	bool fill_;
};

