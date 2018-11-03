#ifndef BRICKBREAKER_SCENE_POWERUP_H_
#define BRICKBREAKER_SCENE_POWERUP_H_

#pragma once
#include "brickbreaker/generic/star.h"

namespace brickbreaker {
class Powerup : public animatedmesh::Star {
 public:
  // Default falling speed of powerup
  float kDefaultFallSpeed = 4.0f;
  // Default rotation speed (in radians)
  float kDefaultRotationSpeed = 0.02f;
  // Default shrinking speed: number between 0 and 1, where a higher value
  // represents a slower shrinking speed (0 is instant, 1 doesn't shrink)
  float kDefaultShrinkingSpeed = 0.8f;
  // Default lifespan of powerup, in seconds
  float kDefaultLifespan = 10.0f;

  // Create star (powerup) with given position, size and color
  Powerup(std::string name, glm::vec3 center, float length, glm::vec3 color);
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
