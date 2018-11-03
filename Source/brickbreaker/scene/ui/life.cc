#include "brickbreaker/scene/ui/life.h"

#include <iostream>

#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
Life::Life(std::string name, glm::vec3 center, float width, glm::vec3 color)
    : Heart(name, center, width, color) {
}

Life::~Life() {}

void Life::Update(float delta_time_seconds) {}
}  // namespace brickbreaker
