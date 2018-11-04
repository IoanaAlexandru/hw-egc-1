#ifndef BRICKBREAKER_SCENE_BALL_H_
#define BRICKBREAKER_SCENE_BALL_H_

#pragma once
#include "brickbreaker/generic/circle.h"

namespace brickbreaker {
class Ball : public animatedmesh::Circle {
 public:
  // Default speed of ball
  float kDefaultSpeed = 8;
  float kSpeedupStep = 4;
  float kMinSpeed = 4;

  // Create circle (ball) with given center, radius and color
  Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
  ~Ball();

  void Update(float delta_time_seconds);
  void StartMoving();
  void Move(float new_position);
  void OnHit(animatedmesh::Position obstacle_position);
  void OnPlatformHit(glm::vec3 platform_top_left_corner, float platform_size);

  inline float GetMovementX() { return movement_x_; }
  inline float GetMovementY() { return movement_y_; }
  inline bool IsMoving() { return is_moving_; }

  inline void SpeedUp() { movement_speed_ += kSpeedupStep; }
  inline void SpeedDown() {
    movement_speed_ -= kSpeedupStep;
    movement_speed_ = std::max(movement_speed_, kMinSpeed);
  }

 private:
  float movement_x_, movement_y_;
  float movement_speed_ = kDefaultSpeed;
  bool is_moving_ = false;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_BALL_H_
