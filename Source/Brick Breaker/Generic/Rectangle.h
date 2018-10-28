#pragma once
#include "AnimatedMesh.h"
class Rectangle :
	public AnimatedMesh
{
public:
	// Create rectangle with given top left corner, size, color and fill
	Rectangle(std::string name, glm::vec3 top_left_corner, float height, float width, glm::vec3 color, bool fill);
	~Rectangle();
	float GetHeight();
	float GetWidth();
	glm::vec3 GetColor();
	bool GetFill();
	glm::vec3 GetTopLeftCorner();

protected:
	float height_, width_;
	glm::vec3 color_;
	bool fill_;
	glm::vec3 top_left_corner_;
	glm::vec3 initial_top_left_corner_;
};

