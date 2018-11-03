#include "brickbreaker/scene/game/wall.h"

#include "Core/Engine.h"

namespace brickbreaker {
Wall::Wall(std::string name, animatedmesh::Position position,
           float scene_height, float scene_width, float thickness,
           glm::vec3 color)
    : Mesh(name) {
  thickness_ = thickness;
  position_ = position;

  glm::vec3 vertex1, vertex2, vertex3, vertex4;

  switch (position) {
    case animatedmesh::UP:
      vertex1 = glm::vec3(0, scene_height - thickness, 0);
      vertex2 = glm::vec3(0, scene_height, 0);
      vertex3 = glm::vec3(scene_width, scene_height, 0);
      vertex4 = glm::vec3(scene_width, scene_height - thickness, 0);
      break;
    case animatedmesh::DOWN:
      vertex1 = glm::vec3(0, 0, 0);
      vertex2 = glm::vec3(0, thickness, 0);
      vertex3 = glm::vec3(scene_width, thickness, 0);
      vertex4 = glm::vec3(scene_width, 0, 0);
      break;
    case animatedmesh::LEFT:
      vertex1 = glm::vec3(0, 0, 0);
      vertex2 = glm::vec3(0, scene_height, 0);
      vertex3 = glm::vec3(thickness, scene_height, 0);
      vertex4 = glm::vec3(thickness, 0, 0);
      break;
    case animatedmesh::RIGHT:
      vertex1 = glm::vec3(scene_width - thickness, 0, 0);
      vertex2 = glm::vec3(scene_width - thickness, scene_height, 0);
      vertex3 = glm::vec3(scene_width, scene_height, 0);
      vertex4 = glm::vec3(scene_width, 0, 0);
      break;
  }

  std::vector<VertexFormat> vertices = {
      VertexFormat(vertex1, color), VertexFormat(vertex2, color),
      VertexFormat(vertex3, color), VertexFormat(vertex4, color)};

  std::vector<unsigned short> indices = {0, 1, 2, 3, 0, 2};

  InitFromData(vertices, indices);
}

Wall::~Wall() {}
}  // namespace brickbreaker
