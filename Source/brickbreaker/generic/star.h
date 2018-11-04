#ifndef BRICKBREAKER_GENERIC_STAR_H_
#define BRICKBREAKER_GENERIC_STAR_H_

#pragma once
#include "brickbreaker/generic/animatedmesh.h"

namespace animatedmesh {
class Star : public AnimatedMesh {
 public:
  // Create star with given center, size and color
  Star(std::string name, glm::vec3 center, float length, glm::vec3 color);
  ~Star();
  inline float GetLength() { return length_; }

 protected:
  float length_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_RECTANGLE_H_
