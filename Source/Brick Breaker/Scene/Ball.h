#pragma once
#include "Brick Breaker/Generic/Circle.h"
class Ball :
	public Circle
{
public:
	// Create circle (ball) with given center, radius and color
	Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Ball();
};
