#include "brickbreaker/scene/platform.h"

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Platform::Platform(std::string name, glm::vec3 top_left_corner, float height,
                   float width, glm::vec3 color, bool fill)
    : Rectangle(name, top_left_corner, height, width, color, fill) {}

Platform::~Platform() {}

void Platform::Update(float delta_time_seconds) {}

/*
Move platform center with new_position. (Platform only moves horizontally)
new_position > 0 => movement to the right
new_position < 0 => movement to the left
*/
void Platform::Move(float new_position) {
  center_.x = initial_center_.x + new_position;
  model_matrix_ = animatedmesh::Translate(new_position, 0);
}
}  // namespace brickbreaker
