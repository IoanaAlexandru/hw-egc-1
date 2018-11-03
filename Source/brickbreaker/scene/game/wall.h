#ifndef BRICKBREAKER_SCENE_WALL_H_
#define BRICKBREAKER_SCENE_WALL_H_

#pragma once

#include "Core/GPU/Mesh.h"
#include "brickbreaker/generic/rectangle.h"

namespace brickbreaker {
class Wall : public Mesh {
 public:
  // Create rectangle (wall) with given thickness and color in the given
  // position using the window sizes
  Wall(std::string name, animatedmesh::Position position, float scene_height,
       float scene_width, float thickness, glm::vec3 color);
  ~Wall();
  inline float GetThickness() { return thickness_; }
  inline animatedmesh::Position GetPosition() { return position_; }

 protected:
  float thickness_;
  animatedmesh::Position position_;
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_WALL_H_
