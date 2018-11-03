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

  inline glm::vec3 GetCenter() { return center_; }
  inline float GetWidth() { return width_; }
  inline glm::vec3 GetColor() { return color_; }

 protected:
  float width_;
  glm::vec3 color_;
  glm::vec3 center_;
  glm::vec3 initial_center_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_HEART_H_
