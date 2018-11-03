#ifndef BRICKBREAKER_SCENE_PAUSEBUTTON_H_
#define BRICKBREAKER_SCENE_PAUSEBUTTON_H_

#pragma once
#include "Core/GPU/Mesh.h"

namespace brickbreaker {
class PauseButton : public Mesh {
 public:
  // Create a white pause button in a square of given length
  PauseButton(std::string name, glm::vec3 top_left_corner, float length);
  ~PauseButton();
};
}  // namespace brickbreaker

#endif  // BRICKBREAKER_SCENE_PAUSEBUTTON_H_