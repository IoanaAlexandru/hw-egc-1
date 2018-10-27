#pragma once
#include "AnimatedMesh.h"
class Circle :
	public AnimatedMesh
{
public:
	// Create circle with given center, radius and color
	Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Circle();

protected:
	float radius_;
	glm::vec3 color_;
};
