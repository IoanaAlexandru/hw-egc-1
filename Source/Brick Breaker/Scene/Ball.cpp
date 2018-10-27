#include "Ball.h"

Ball::Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color)
	: Circle(name, center, radius, color)
{
}

Ball::~Ball()
{
}
