#include "Platform.h"

#include <Core/Engine.h>

Platform::Platform(std::string name, glm::vec3 bottomLeftCorner, float height, float width, glm::vec3 color, bool fill)
	: Mesh(name)
{
	glm::vec3 corner = bottomLeftCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	std::vector<unsigned short> indices = { 0, 1, 2, 3, 0, 2 };

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

Platform::~Platform()
{
}
