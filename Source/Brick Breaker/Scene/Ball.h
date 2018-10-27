#pragma once
#include "Core/GPU/Mesh.h"
class Ball :
	public Mesh
{
public:
	// Create circle (ball) with given center, radius and color
	Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Ball();
};
