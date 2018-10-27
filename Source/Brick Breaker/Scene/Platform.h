#pragma once
#include "Core/GPU/Mesh.h"
class Platform :
	public Mesh
{
public:
	// Create rectangle (platform) with given bottom left corner, size and color
	Platform(std::string name, glm::vec3 bottomLeftCorner, float height, float width, glm::vec3 color, bool fill);
	~Platform();
};
