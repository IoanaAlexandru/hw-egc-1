#include "Brick.h"

#include <Core/Engine.h>

Brick::Brick(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill)
	: Mesh(name)
{
	glm::vec3 corner = topLeftCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, -height, 0), color),
		VertexFormat(corner + glm::vec3(0, -height, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	InitFromData(vertices, indices);
}


Brick::~Brick()
{
}
