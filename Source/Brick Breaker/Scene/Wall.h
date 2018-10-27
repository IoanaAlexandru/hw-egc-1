#pragma once
#include "Core/GPU/Mesh.h"

enum Position { UP, DOWN, LEFT, RIGHT };

class Wall :
	public Mesh
{
public:
	// Create rectangle (wall) with given thickness and color in the given position using the window sizes
	Wall(std::string name, Position position, float scene_height, float scene_width, float thickness, glm::vec3 color);
	~Wall();
};
