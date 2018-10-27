#include "Circle.h"

Circle::Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color)
	: Mesh(name)
{
	float theta = 0;
	float x, y;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.push_back(VertexFormat(center, color));

	while (theta <= 6.5) {
		x = radius * cos(theta);
		y = radius * sin(theta);
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0) + center, color));
		theta += 0.1;
	}

	for (int i = 1; i < vertices.size() - 1; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	InitFromData(vertices, indices);
}

Circle::~Circle()
{
}
