#include "brickbreaker/generic/rectangle.h"
#include "Core/Engine.h"

namespace animatedmesh {
Rectangle::Rectangle(std::string name, glm::vec3 top_left_corner, float height,
                     float width, glm::vec3 color, bool fill)
    : AnimatedMesh(name) {
  height_ = height;
  width_ = width;
  color_ = color;
  fill_ = fill;
  center_ = top_left_corner + glm::vec3(width / 2, -height / 2, 0);
  initial_center_ = center_;

  glm::vec3 corner = top_left_corner;

  std::vector<VertexFormat> vertices = {
      VertexFormat(corner, color),
      VertexFormat(corner + glm::vec3(width, 0, 0), color),
      VertexFormat(corner + glm::vec3(width, -height, 0), color),
      VertexFormat(corner + glm::vec3(0, -height, 0), color)};

  std::vector<unsigned short> indices = {0, 1, 2, 3};

  if (!fill) {
    SetDrawMode(GL_LINE_LOOP);
  } else {
    // draw 2 triangles. Add the remaining 2 indices
    indices.push_back(0);
    indices.push_back(2);
  }

  InitFromData(vertices, indices);
}

Rectangle::~Rectangle() {}
}  // namespace animatedmesh
