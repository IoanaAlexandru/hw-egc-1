#include "brickbreaker/generic/heart.h"

namespace animatedmesh {
Heart::Heart(std::string name, glm::vec3 center, float width, glm::vec3 color)
    : AnimatedMesh(name) {
  color_ = color;
  center_ = center;
  initial_center_ = center;

  float theta = 0;
  float x, y;
  float radius = width / 4;

  std::vector<VertexFormat> vertices;
  std::vector<unsigned short> indices;

  // Left semicircle
  int left_semicircle_center_index = 0;
  glm::vec3 left_semicircle_center = center + glm::vec3(-radius, 0, 0);
  vertices.push_back(VertexFormat(left_semicircle_center, color));

  while (theta <= 3.14) {
    x = radius * cos(theta);
    y = radius * sin(theta);
    vertices.push_back(
        VertexFormat(glm::vec3(x, y, 0) + left_semicircle_center, color));
    theta += 0.1;
  }

  for (int i = left_semicircle_center_index + 1; i < vertices.size() - 1; i++) {
    indices.push_back(left_semicircle_center_index);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  // Right semicircle
  int right_semicircle_center_index = vertices.size();
  glm::vec3 right_semicircle_center = center + glm::vec3(radius, 0, 0);
  vertices.push_back(VertexFormat(right_semicircle_center, color));

  theta = 0;
  while (theta <= 3.14) {
    x = radius * cos(theta);
    y = radius * sin(theta);
    vertices.push_back(
        VertexFormat(glm::vec3(x, y, 0) + right_semicircle_center, color));
    theta += 0.1;
  }

  for (int i = right_semicircle_center_index + 1; i < vertices.size() - 1;
       i++) {
    indices.push_back(right_semicircle_center_index);
    indices.push_back(i);
    indices.push_back(i + 1);
  }

  // Bottom triangle
  int next_index = vertices.size();
  glm::vec3 middle_left = center + glm::vec3(-width / 2, 0, 0);
  vertices.push_back(VertexFormat(middle_left, color));
  glm::vec3 middle_bottom = center + glm::vec3(0, -width / 2, 0);
  vertices.push_back(VertexFormat(middle_bottom, color));
  glm::vec3 middle_right = center + glm::vec3(width / 2, 0, 0);
  vertices.push_back(VertexFormat(middle_right, color));

  indices.push_back(next_index);
  indices.push_back(next_index + 1);
  indices.push_back(next_index + 2);

  InitFromData(vertices, indices);
}

Heart::~Heart() {}
}  // namespace animatedmesh
