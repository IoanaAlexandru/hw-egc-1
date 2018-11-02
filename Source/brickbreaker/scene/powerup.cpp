#include "brickbreaker/scene/powerup.h"

#include <iostream>

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Powerup::Powerup(std::string name, glm::vec3 top_left_corner, float length,
                 glm::vec3 color, bool fill)
    : Rectangle(name, top_left_corner, length, length, color, fill) {}

Powerup::~Powerup() {}

void Powerup::Update(float delta_time_seconds) {
  if (is_shrinking_) {
    lifespan_ -= delta_time_seconds;
    model_matrix_ *=
        animatedmesh::Translate(initial_center_.x, initial_center_.y);
    model_matrix_ *= animatedmesh::Scale(shrinking_speed_, shrinking_speed_);
    model_matrix_ *=
        animatedmesh::Translate(-initial_center_.x, -initial_center_.y);
  }
  center_ += glm::vec3(0, -fall_speed_, 0);
  angle_ += rotation_speed_;
  model_matrix_ = animatedmesh::Translate(center_.x, center_.y);
  model_matrix_ *= animatedmesh::Rotate(angle_);
  model_matrix_ *=
      animatedmesh::Translate(-initial_center_.x, -initial_center_.y);
}

void Powerup::OnPlatformHit(glm::vec3 platform_center, float platform_size) {
  if (abs(center_.x - platform_center.x) > platform_size / 2)
    return;  // powerup doesn't touch the platform
  is_shrinking_ = true;
}
}  // namespace brickbreaker
