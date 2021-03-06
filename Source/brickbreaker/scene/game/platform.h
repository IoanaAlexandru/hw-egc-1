#ifndef BRICKBREAKER_SCENE_PLATFORM_H_
#define BRICKBREAKER_SCENE_PLATFORM_H_

#pragma once
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Platform : public animatedmesh::Rectangle {
 public:
  // Create rectangle (platform) with given top left corner, size and color
  Platform(std::string name, glm::vec3 top_left_corner, float height,
           float width, glm::vec3 color, bool fill);
  ~Platform();

  // The number of times that the width shrinks/stretches
  float kResizeRate = 1.5;

  void Update(float delta_time_seconds);
  void Move(float mouse_position);

  inline bool IsSticky() { return is_sticky_; }
  inline void MakeSticky() { is_sticky_ = true; }
  inline void MakeNotSticky() { is_sticky_ = false; }

  void ShrinkWidth();
  void StretchWidth();

  glm::mat3 scale_matrix_ = glm::mat3(1);
  bool is_sticky_;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_PLATFORM_H_
