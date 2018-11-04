#ifndef BRICKBREAKER_SCENE_BRICK_H_
#define BRICKBREAKER_SCENE_BRICK_H_

#pragma once
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Brick : public animatedmesh::Rectangle {
 public:
  // Default shrinking speed: number between 0 and 50, where a higher value
  // represents a slower shrinking speed
  float kDefaultShrinkingSpeed = 50.0f;

  // Create rectangle (brick) with given top left corner, size, color and fill
  // If brick doesn't have fill, it is not solid (ball can pass through it)
  Brick(std::string name, glm::vec3 top_left_corner, float height, float width,
        glm::vec3 color, bool fill);
  ~Brick();

  inline bool IsSolid() { return !is_shrinking_ && is_solid_; }
  inline bool IsShrinking() { return is_shrinking_; }

  void Update(float delta_time_seconds);
  void OnHit();

 private:
  bool is_shrinking_ = false;
  float shrinking_speed_ = kDefaultShrinkingSpeed;
  bool is_solid_ = true;

  void Shrink();
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_BRICK_H_
