#ifndef BRICKBREAKER_SCENE_BRICK_H_
#define BRICKBREAKER_SCENE_BRICK_H_

#pragma once
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Brick : public animatedmesh::Rectangle {
 public:
  // Create rectangle (brick) with given top left corner, size and color
  Brick(std::string name, glm::vec3 top_left_corner, float height, float width,
        glm::vec3 color, bool fill);
  ~Brick();

  void Update(float delta_time_seconds);
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_BRICK_H_
