#ifndef BRICKBREAKER_SCENE_BALL_H_
#define BRICKBREAKER_SCENE_BALL_H_

#pragma once
#include "brickbreaker/generic/circle.h"

namespace brickbreaker {
class Ball : public animatedmesh::Circle {
 public:
  // Default speed of ball
  float kDefaultSpeed = 400;
  float kSpeedupStep = 200;
  float kMinSpeed = 200;

  // Create circle (ball) with given center, radius and color
  Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
  ~Ball();

  void Update(float delta_time_seconds);
  void StartMoving(glm::vec3 platform_center, float platform_width,
                   float platform_height);
  void Move(float new_position);
  void OnHit(animatedmesh::Position obstacle_position);
  void OnPlatformHit(glm::vec3 platform_center, float platform_width,
                     float platform_height, bool sticky_platform);

  inline float GetMovementX() { return movement_x_; }
  inline float GetMovementY() { return movement_y_; }
  inline bool IsMoving() { return is_moving_; }
  inline bool IsPoweredUp() { return is_powered_up_; }

  inline void SpeedUp() {
    float new_movement_speed = movement_speed_ + kSpeedupStep;
    movement_x_ = movement_x_ * new_movement_speed / movement_speed_;
    movement_y_ = movement_y_ * new_movement_speed / movement_speed_;
    movement_speed_ = new_movement_speed;
  }
  inline void SlowDown() {
    float new_movement_speed =
        std::max(movement_speed_ - kSpeedupStep, kMinSpeed);
    movement_x_ = movement_x_ * new_movement_speed / movement_speed_;
    movement_y_ = movement_y_ * new_movement_speed / movement_speed_;
    movement_speed_ = new_movement_speed;
  }

  inline void PowerUp() { is_powered_up_ = true; }
  inline void PowerDown() { is_powered_up_ = false; }

 private:
  float movement_x_, movement_y_;
  float movement_speed_ = kDefaultSpeed;
  bool is_moving_ = false;
  bool is_powered_up_ = false;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_BALL_H_
