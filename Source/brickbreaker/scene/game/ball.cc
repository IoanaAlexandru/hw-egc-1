#include "brickbreaker/scene/game/ball.h"

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
    model_matrix_ = animatedmesh::Translate(center_.x - initial_center_.x,
                                            center_.y - initial_center_.y);
    center_ += glm::vec3(movement_x_ * delta_time_seconds,
                         movement_y_ * delta_time_seconds, 0);
  }
}

void Ball::StartMoving(glm::vec3 platform_center, float platform_width, float platform_height) {
  OnPlatformHit(platform_center, platform_width, platform_height, false);
}

/*
Move ball center horizontally with new_position.

new_position > 0 => movement to the right
new_position < 0 => movement to the left
*/
void Ball::Move(float new_position) {
  center_.x += new_position;
  model_matrix_ = animatedmesh::Translate(center_.x - initial_center_.x,
                                          center_.y - initial_center_.y);
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

void Ball::OnPlatformHit(glm::vec3 platform_center, float platform_width,
                         float platform_height, bool sticky_platform) {
  /*
  Middle of the platform: cosine = 0
  Rightmost corner: cosine = 1
  Leftmost corner: cosine = -1
  Anything in between: calculated linear value
  */
  float cosine = (center_.x - platform_center.x) / (platform_width / 2);
  if (cosine < -1 || cosine > 1) return;  // ball doesn't touch the platform

  if (sticky_platform) {
    is_moving_ = false;
    return;
  }

  is_moving_ = true;

  float reflect_angle = acos(cosine);

  movement_x_ = movement_speed_ * cos(reflect_angle);
  movement_y_ = movement_speed_ * sin(reflect_angle);

  // Make sure the ball didn't end up below the platform
  center_.y =
      std::max(platform_center.y + platform_height + radius_ + 0.1f, center_.y);
  Update(0);
}
}  // namespace brickbreaker
