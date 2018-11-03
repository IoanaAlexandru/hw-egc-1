#ifndef BRICKBREAKER_SCENE_LIFE_H_
#define BRICKBREAKER_SCENE_LIFE_H_

#pragma once
#include "brickbreaker/generic/circle.h"

namespace brickbreaker {
class Life : public animatedmesh::Circle {
 public:
  // Create circle (life) with given center, radius and color
  Life(std::string name, glm::vec3 center, float radius, glm::vec3 color);
  ~Life();

  void Update(float delta_time_seconds);
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_LIFE_H_
