#include "brickbreaker/generic/star.h"
#include "Core/Engine.h"

namespace animatedmesh {
Star::Star(std::string name, glm::vec3 center, float length, glm::vec3 color)
    : AnimatedMesh(name) {
  color_ = color;
  center_ = center;
  initial_center_ = center_;
  length_ = length;

  std::vector<VertexFormat> vertices = {
      VertexFormat(center + glm::vec3(-length / 2, length / 4, 0), color),
      VertexFormat(center + glm::vec3(length / 2, length / 4, 0), color),
      VertexFormat(center + glm::vec3(0, -length / 2, 0), color),
      VertexFormat(center + glm::vec3(-length / 2, -length / 4, 0), color),
      VertexFormat(center + glm::vec3(length / 2, -length / 4, 0), color),
      VertexFormat(center + glm::vec3(0, length / 2, 0), color)};

  std::vector<unsigned short> indices = {0, 1, 2, 3, 4, 5};

  InitFromData(vertices, indices);
}

Star::~Star() {}
}  // namespace animatedmesh
