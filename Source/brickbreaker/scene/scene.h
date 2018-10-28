#ifndef BRICKBREAKER_SCENE_SCENE_H_
#define BRICKBREAKER_SCENE_SCENE_H_

#pragma once

#include <map>
#include <string>

#include "Component/SimpleScene.h"
#include "Core/Engine.h"
#include "brickbreaker/scene/ball.h"
#include "brickbreaker/scene/brick.h"
#include "brickbreaker/scene/pausebutton.h"
#include "brickbreaker/scene/platform.h"
#include "brickbreaker/scene/wall.h"

namespace brickbreaker {
class Scene : public SimpleScene {
 public:
  Scene();
  ~Scene();

  void Init() override;

 private:
  void FrameStart() override;
  void Update(float deltaTimeSeconds) override;
  void FrameEnd() override;

  void OnInputUpdate(float deltaTime, int mods) override;
  void OnKeyPress(int key, int mods) override;
  void OnKeyRelease(int key, int mods) override;
  void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
  void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
  void OnWindowResize(int width, int height) override;

 protected:
  // percentage of scene width reserved for brick panel
  static const float kBrickPanelWidthRatio;
  // percentage of scene height reserved for brick panel
  static const float kBrickPanelHeightRatio;
  // platform width as a percentage of scene width
  static const float kPlatformWidthRatio;
  // platform height as a percentage of platform width
  static const float kPlatformHeightToWidthRatio;
  // wall thickness as a percentage of the smallest scene dimension (width or height)
  static const float kWallThicknessRatio;
  // percentage of brick panel reserved for the space between bricks
  static const float kBrickDistanceRatio;
  // ball diameter as a percentage of the platform width
  static const float kBallToPlatformRatio;
  static const int kBricksPerRow, kBrickRows;
  static const float kPauseButtonSize;

  std::vector<std::vector<Brick *>> bricks_;
  std::map<animatedmesh::Position, Wall *> walls_;
  Platform *platform_;
  std::vector<Ball *> balls_;
  PauseButton *pause_button_;

  float scene_width_, scene_height_;

  bool paused_ = false;
};
}  // namespace brickbreaker

#endif BRICKBREAKER_SCENE_SCENE_H_
