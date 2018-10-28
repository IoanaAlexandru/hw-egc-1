#ifndef BRICKBREAKER_GENERIC_RECTANGLE_H_
#define BRICKBREAKER_GENERIC_RECTANGLE_H_

#pragma once
#include "brickbreaker/generic/animatedmesh.h"

namespace animatedmesh {
class Rectangle : public AnimatedMesh {
 public:
  // Create rectangle with given top left corner, size, color and fill
  Rectangle(std::string name, glm::vec3 top_left_corner, float height,
            float width, glm::vec3 color, bool fill);
  ~Rectangle();
  inline float GetHeight() { return height_; }
  inline float GetWidth() { return width_; }
  inline glm::vec3 GetColor() { return color_; }
  inline bool GetFill() { return fill_; }
  inline glm::vec3 GetCenter() { return center_; }

 protected:
  float height_, width_;
  glm::vec3 color_;
  bool fill_;
  glm::vec3 center_;
  glm::vec3 initial_center_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_RECTANGLE_H_
