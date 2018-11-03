#ifndef BRICKBREAKER_GENERIC_STAR_H_
#define BRICKBREAKER_GENERIC_STAR_H_

#pragma once
#include "brickbreaker/generic/animatedmesh.h"

namespace animatedmesh {
class Star : public AnimatedMesh {
 public:
  // Create star with given top left corner, size and color
  Star(std::string name, glm::vec3 center, float length, glm::vec3 color);
  ~Star();
  inline float GetLength() { return length_; }
  inline glm::vec3 GetColor() { return color_; }
  inline glm::vec3 GetCenter() { return center_; }

 protected:
  float length_;
  glm::vec3 color_;
  glm::vec3 center_;
  glm::vec3 initial_center_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_RECTANGLE_H_
