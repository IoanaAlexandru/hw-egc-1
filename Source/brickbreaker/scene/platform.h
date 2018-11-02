#ifndef BRICKBREAKER_SCENE_PLATFORM_H_
#define BRICKBREAKER_SCENE_PLATFORM_H_

#pragma once
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Platform : public animatedmesh::Rectangle {
 public:
  // Create rectangle (platform) with given bottom left corner, size and color
  Platform(std::string name, glm::vec3 top_left_corner, float height,
           float width, glm::vec3 color, bool fill);
  ~Platform();

  // The number of times that the width shrinks/stretches
  float kResizeRate = 1.5;

  void Update(float delta_time_seconds);
  void Move(float mouse_position);

  void ShrinkWidth();
  void StretchWidth();

  glm::mat3 scale_matrix_ = glm::mat3(1);
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_PLATFORM_H_
