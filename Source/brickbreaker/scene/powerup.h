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
  float kDefaultRotationSpeed = 0.05;
  // Default shrinking speed: number between 0 and 1, where a higher value
  // represents a slower shrinking speed (0 is instant, 1 doesn't shrink)
  double kDefaultShrinkingSpeed = 0.8;
  // Default lifespan of powerup, in seconds
  double kDefaultLifespan = 30;

  // Create square (powerup) with given position, size, color and fill
  Powerup(std::string name, glm::vec3 top_left_corner, float length,
          glm::vec3 color, bool fill);
  ~Powerup();

  void Update(float delta_time_seconds);
  void OnPlatformHit(glm::vec3 platform_top_left_corner, float platform_size);

  inline bool IsActive() { return is_shrinking_ && lifespan_ > 0; }
  inline bool IsActivated() { return is_shrinking_; }

 private:
  float fall_speed_ = kDefaultFallSpeed;
  float rotation_speed_ = kDefaultRotationSpeed;
  float shrinking_speed_ = kDefaultShrinkingSpeed;
  float angle_ = 0;

  float lifespan_ = kDefaultLifespan;
  bool is_shrinking_ = false;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_POWERUP_H_
