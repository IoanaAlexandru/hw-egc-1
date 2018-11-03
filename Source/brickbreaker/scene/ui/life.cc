#include "brickbreaker/scene/ui/life.h"

#include <iostream>

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Life::Life(std::string name, glm::vec3 center, float radius, glm::vec3 color)
    : Circle(name, center, radius, color) {
  center_ = center;
}

Life::~Life() {}

void Life::Update(float delta_time_seconds) {}
}  // namespace brickbreaker
