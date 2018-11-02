#ifndef BRICKBREAKER_SCENE_POWERUP_H_
#define BRICKBREAKER_SCENE_POWERUP_H_

#pragma once
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Powerup : public animatedmesh::Rectangle {
 public:
  // Default falling speed of powerup
  float kDefaultFallSpeed = 4;
  // Default rotation speed (in radians)
  float kDefaultRotationSpeed = 10;
  // Default shrinking speed: number between 0 and 1, where a higher value
  // represents a slower shrinking speed (0 is instant, 1 doesn't shrink)
  double kDefaultShrinkingSpeed = 0.8;

  // Create square (powerup) with given position, size, color and fill
  Powerup(std::string name, glm::vec3 top_left_corner, float length,
          glm::vec3 color, bool fill);
  ~Powerup();

  void Update(float delta_time_seconds);
  void OnPlatformHit(glm::vec3 platform_top_left_corner, float platform_size);

 private:
  float fall_speed_ = kDefaultFallSpeed;
  float rotation_speed_ = kDefaultRotationSpeed;
  float shrinking_speed_ = kDefaultShrinkingSpeed;

  bool is_shrinking_ = false;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_POWERUP_H_
