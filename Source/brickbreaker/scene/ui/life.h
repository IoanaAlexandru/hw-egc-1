#ifndef BRICKBREAKER_SCENE_LIFE_H_
#define BRICKBREAKER_SCENE_LIFE_H_

#pragma once
#include "brickbreaker/generic/heart.h"

namespace brickbreaker {
class Life : public animatedmesh::Heart {
 public:
  // Create heart (life) with given center, width and color
  Life(std::string name, glm::vec3 center, float width, glm::vec3 color);
  ~Life();

  void Update(float delta_time_seconds);
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_LIFE_H_
