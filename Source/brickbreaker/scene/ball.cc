#include "brickbreaker/scene/ball.h"

#include <iostream>

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Ball::Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color)
    : Circle(name, center, radius, color) {
  center_ = center;
}

Ball::~Ball() {}

void Ball::Update(float delta_time_seconds) {
  if (is_moving_) {
    model_matrix_ *= animatedmesh::Translate(movement_x_, movement_y_);
    center_ += glm::vec3(movement_x_, movement_y_, 0);
  }
}

void Ball::StartMoving() {
  if (!is_moving_) {
    is_moving_ = true;
    movement_y_ = movement_speed_;
  }
}

void Ball::Move(float new_position) {
  if (!is_moving_) {
    center_.x = initial_center_.x + new_position;
    model_matrix_ = animatedmesh::Translate(new_position, 0);
  }
}

void Ball::OnHit(animatedmesh::Position obstacle_position) {
  switch (obstacle_position) {
    case animatedmesh::UP:
    case animatedmesh::DOWN:
      movement_y_ = -movement_y_;
      break;
    case animatedmesh::LEFT:
    case animatedmesh::RIGHT:
      movement_x_ = -movement_x_;
      break;
  }
}

void Ball::OnPlatformHit(glm::vec3 platform_center, float platform_size) {
  /*
  Middle of the platform: cosine = 0
  Rightmost corner: cosine = 1
  Leftmost corner: cosine = -1
  Anything in between: calculated linear value
  */
  float cosine = (center_.x - platform_center.x) / (platform_size / 2);
  if (cosine < -1 || cosine > 1) return;  // ball doesn't touch the platform

  float reflect_angle = acos(cosine);

  movement_x_ = movement_speed_ * cos(reflect_angle);
  movement_y_ = movement_speed_ * sin(reflect_angle);
}
}  // namespace brickbreaker
