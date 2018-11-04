#include "brickbreaker/scene/game/brick.h"
#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Brick::Brick(std::string name, glm::vec3 top_left_corner, float height,
             float width, glm::vec3 color, bool fill)
    : Rectangle(name, top_left_corner, height, width, color, fill) {
  is_solid_ = fill;
}

Brick::~Brick() {}

void Brick::Update(float delta_time_seconds) {
  if (is_shrinking_) {
    model_matrix_ *=
        animatedmesh::Translate(initial_center_.x, initial_center_.y);
    float shrinking_speed =
        std::min(shrinking_speed_ * delta_time_seconds, 1.0f);
    model_matrix_ *= animatedmesh::Scale(shrinking_speed, shrinking_speed);
    model_matrix_ *=
        animatedmesh::Translate(-initial_center_.x, -initial_center_.y);
  }
}

void Brick::Shrink() { is_shrinking_ = true; }

void Brick::OnHit() { Shrink(); }
}  // namespace brickbreaker
