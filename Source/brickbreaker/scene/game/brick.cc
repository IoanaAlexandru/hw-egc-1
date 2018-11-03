#include "brickbreaker/scene/game/brick.h"
#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Brick::Brick(std::string name, glm::vec3 top_left_corner, float height,
             float width, glm::vec3 color, bool fill)
    : Rectangle(name, top_left_corner, height, width, color, fill) {}

Brick::~Brick() {}

void Brick::Update(float delta_time_seconds) {
  if (is_shrinking_) {
    model_matrix_ *= animatedmesh::Translate(initial_center_.x,
                                             initial_center_.y);
    model_matrix_ *= animatedmesh::Scale(shrinking_speed_, shrinking_speed_);
    model_matrix_ *= animatedmesh::Translate(-initial_center_.x,
                                             -initial_center_.y);
  }
}

void Brick::Shrink() {
  is_shrinking_ = true;
}

void Brick::OnHit() { Shrink(); }
}  // namespace brickbreaker
