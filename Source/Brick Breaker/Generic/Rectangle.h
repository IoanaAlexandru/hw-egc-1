#pragma once
#include "Core/GPU/Mesh.h"
class Rectangle :
	public Mesh
{
public:
	// Create rectangle with given top left corner, size, color and fill
	Rectangle(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill);
	~Rectangle();
};

