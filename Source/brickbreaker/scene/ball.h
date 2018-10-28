#ifndef BRICKBREAKER_SCENE_BALL_H_
#define BRICKBREAKER_SCENE_BALL_H_

#pragma once
#include "brickbreaker/generic/circle.h"

namespace brickbreaker {
class Ball : public animatedmesh::Circle {
 public:
  // Default speed of ball
  float kDefaultSpeed = 10;

  // Create circle (ball) with given center, radius and color
  Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
  ~Ball();

  void Update(float delta_time_seconds);
  void StartMoving();
  void Move(float new_position);
  void OnWallCollision(animatedmesh::Position wall_position);
  void OnPlatformHit(glm::vec3 platform_top_left_corner, float platform_size);

 private:
  float movement_x_, movement_y_;
  float movement_speed_;
  bool is_moving_;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_BALL_H_
