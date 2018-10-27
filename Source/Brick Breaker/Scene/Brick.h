#pragma once
#include "Core/GPU/Mesh.h"
class Brick :
	public Mesh
{
public:
	// Create rectangle (brick) with given bottom left corner, length and color
	Brick(std::string name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill);
	~Brick();
};
