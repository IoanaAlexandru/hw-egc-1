#ifndef BRICKBREAKER_GENERIC_HEART_H_
#define BRICKBREAKER_GENERIC_HEART_H_

#pragma once
#include "brickbreaker/generic/animatedmesh.h"

namespace animatedmesh {
class Heart : public AnimatedMesh {
 public:
  // Create heart shape with given center, width and color
  Heart(std::string name, glm::vec3 center, float width, glm::vec3 color);
  ~Heart();

  inline float GetWidth() { return width_; }

 protected:
  float width_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_HEART_H_
