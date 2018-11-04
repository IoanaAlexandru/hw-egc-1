#ifndef BRICKBREAKER_GENERIC_ANIMATEDMESH_H_
#define BRICKBREAKER_GENERIC_ANIMATEDMESH_H_

#pragma once
#include "Core/GPU/Mesh.h"

namespace animatedmesh {
enum Position { UP, DOWN, LEFT, RIGHT };

class AnimatedMesh : public Mesh {
 public:
  AnimatedMesh(std::string name) : Mesh(name){};
  ~AnimatedMesh(){};

  // Update model matrix and fields
  virtual void Update(float delta_time_seconds) = 0;

  inline glm::vec3 GetCenter() { return center_; }
  inline glm::mat3 GetModelMatrix() { return model_matrix_; }
  inline glm::vec3 GetColor() { return color_; }

 protected:
  glm::mat3 model_matrix_ = glm::mat3(1);
  // current center of shape
  glm::vec3 center_;
  // initial center of shape
  glm::vec3 initial_center_;
  glm::vec3 color_;
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_ANIMATEDMESH_H_
