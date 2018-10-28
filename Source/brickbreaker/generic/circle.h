#ifndef BRICKBREAKER_GENERIC_CIRCLE_H_
#define BRICKBREAKER_GENERIC_CIRCLE_H_

#pragma once
#include "brickbreaker/generic/animatedmesh.h"

namespace animatedmesh {
class Circle : public AnimatedMesh {
 public:
  // Create circle with given center, radius and color
  Circle(std::string name, glm::vec3 center, float radius, glm::vec3 color);
  ~Circle();

  inline glm::vec3 GetCenter() { return center_; }
  inline float GetRadius() { return radius_; }
  inline glm::vec3 GetColor() { return color_; }

 protected:
  float radius_;
  glm::vec3 color_;
  glm::vec3 center_;
  glm::vec3 initial_center_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_CIRCLE_H_
