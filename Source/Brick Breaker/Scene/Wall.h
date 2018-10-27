#pragma once
#include "Core/GPU/Mesh.h"

enum Position { UP, DOWN, LEFT, RIGHT };

class Wall :
	public Mesh
{
public:
	// Create rectangle (brick) with given bottom left corner, length and color
	Wall(std::string name, Position position, float window_height, float window_width, float thickness, glm::vec3 color);
	~Wall();
};
