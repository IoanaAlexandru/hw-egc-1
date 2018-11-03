#include "brickbreaker/scene/game/platform.h"

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Platform::Platform(std::string name, glm::vec3 top_left_corner, float height,
                   float width, glm::vec3 color, bool fill)
    : Rectangle(name, top_left_corner, height, width, color, fill) {}

Platform::~Platform() {}

void Platform::Update(float delta_time_seconds) {
  model_matrix_ = animatedmesh::Translate(center_.x, center_.y);
  model_matrix_ *= scale_matrix_;
  model_matrix_ *=
      animatedmesh::Translate(-initial_center_.x, -initial_center_.y);
}

/*
Move platform center with new_position. (Platform only moves horizontally)

new_position > 0 => movement to the right
new_position < 0 => movement to the left
*/
void Platform::Move(float new_position) {
  center_.x += new_position;
  model_matrix_ = animatedmesh::Translate(center_.x, center_.y);
  model_matrix_ *= scale_matrix_;
  model_matrix_ *=
      animatedmesh::Translate(-initial_center_.x, -initial_center_.y);
}

void Platform::ShrinkWidth() {
  width_ /= kResizeRate;
  scale_matrix_ *= animatedmesh::Scale(1 / kResizeRate, 1);
}

void Platform::StretchWidth() {
  width_ *= kResizeRate;
  scale_matrix_ *= animatedmesh::Scale(kResizeRate, 1);
}
}  // namespace brickbreaker
