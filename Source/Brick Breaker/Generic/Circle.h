#pragma once
#include "Core/GPU/Mesh.h"
class Circle :
	public Mesh
{
public:
	// Create circle with given center, radius and color
	Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Circle();
};
