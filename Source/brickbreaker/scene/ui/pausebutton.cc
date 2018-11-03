#include "brickbreaker/scene/ui/pausebutton.h"

#include "Core/Engine.h"

namespace brickbreaker {
PauseButton::PauseButton(std::string name, glm::vec3 top_left_corner,
                         float length)
    : Mesh(name) {
  glm::vec3 color = glm::vec3(1, 1, 1);
  glm::vec3 corner = top_left_corner;

  std::vector<VertexFormat> vertices = {
      VertexFormat(corner, color),
      VertexFormat(corner + glm::vec3(length / 3, 0, 0), color),
      VertexFormat(corner + glm::vec3(length / 3, -length, 0), color),
      VertexFormat(corner + glm::vec3(0, -length, 0), color),
      VertexFormat(corner + glm::vec3(length / 3 * 2, 0, 0), color),
      VertexFormat(corner + glm::vec3(length, 0, 0), color),
      VertexFormat(corner + glm::vec3(length, -length, 0), color),
      VertexFormat(corner + glm::vec3(length / 3 * 2, -length, 0), color)};

  std::vector<unsigned short> indices = {0, 1, 2, 3, 0, 2, 4, 5, 6, 7, 4, 6};

  InitFromData(vertices, indices);
}

PauseButton::~PauseButton() {}
}  // namespace brickbreaker
