#ifndef BRICKBREAKER_GENERIC_TRANSFORM2D_H_
#define BRICKBREAKER_GENERIC_TRANSFORM2D_H_

#pragma once
#include "include/glm.h"
namespace animatedmesh {
// Translate matrix
inline glm::mat3 Translate(float translate_x, float translate_y) {
  return glm::transpose(
      glm::mat3(1, 0, translate_x,
                0, 1, translate_y,
                0, 0,           1));
}

// Scale matrix
inline glm::mat3 Scale(float scale_x, float scale_y) {
  return glm::transpose(glm::mat3(scale_x,       0, 0,
                                        0, scale_y, 0,
                                        0,       0, 1));
}

// Rotate matrix
inline glm::mat3 Rotate(float radians) {
  return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0,
                                  sin(radians),  cos(radians), 0,
                                             0,             0, 1));
}
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_TRANSFORM2D_H_
