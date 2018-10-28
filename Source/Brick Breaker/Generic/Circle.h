#pragma once
#include "AnimatedMesh.h"
class Circle :
	public AnimatedMesh
{
public:
	// Create circle with given center, radius and color
	Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Circle();
	glm::vec3 GetCenter();
	float GetRadius();
	glm::vec3 GetColor();

protected:
	float radius_;
	glm::vec3 color_;
	glm::vec3 center_;
	glm::vec3 initial_center_;
};
