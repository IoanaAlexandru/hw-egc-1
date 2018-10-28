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

  virtual void Update(float delta_time_seconds) = 0;

  inline glm::mat3 GetModelMatrix() { return model_matrix_; }

 protected:
  glm::mat3 model_matrix_ = glm::mat3(1);
};
}  // namespace animatedmesh

#endif  // BRICKBREAKER_GENERIC_ANIMATEDMESH_H_
