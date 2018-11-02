#ifndef BRICKBREAKER_SCENE_SCENE_H_
#define BRICKBREAKER_SCENE_SCENE_H_

#pragma once

#include <map>
#include <random>
#include <string>

#include "Component/SimpleScene.h"
#include "Core/Engine.h"
#include "brickbreaker/scene/ball.h"
#include "brickbreaker/scene/brick.h"
#include "brickbreaker/scene/pausebutton.h"
#include "brickbreaker/scene/platform.h"
#include "brickbreaker/scene/powerup.h"
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

  void InitPauseButton();
  void InitWalls();
  void InitBrickPanel();
  void InitPlatform();
  void InitBall();

  std::default_random_engine generator_;
  std::bernoulli_distribution distribution_;
  inline bool ShouldSpawnPowerup() { return distribution_(generator_); }
  void SpawnPowerup(glm::vec3 top_left_corner);

 protected:
  // percentage of scene width reserved for brick panel
  static const float kBrickPanelWidthRatio;
  // percentage of scene height reserved for brick panel
  static const float kBrickPanelHeightRatio;
  // platform width as a percentage of scene width
  static const float kPlatformWidthRatio;
  // platform height as a percentage of platform width
  static const float kPlatformHeightToWidthRatio;
  // wall thickness as a percentage of the smallest scene dimension
  static const float kWallThicknessRatio;
  // percentage of brick panel reserved for the space between bricks
  static const float kBrickDistanceRatio;
  // ball diameter as a percentage of the platform width
  static const float kBallToPlatformRatio;
  static const int kBricksPerRow, kBrickRows;
  static const float kPauseButtonSize;
  static const int kMaxLives;
  // chance of a powerup to spawn after a brick is destroyed
  static const float kPowerupChance;
  static const float kPowerupSize;

  std::vector<std::vector<Brick *>> bricks_;
  std::map<animatedmesh::Position, Wall *> walls_;
  Platform *platform_;
  std::vector<Ball *> balls_;
  std::vector<Powerup *> powerups_;
  PauseButton *pause_button_;

  float scene_width_, scene_height_;
  float wall_thickness_;
  float brick_width_, brick_height_;
  float platform_width_, platform_height_;

  bool paused_ = false;
  int lives_ = kMaxLives;
};
}  // namespace brickbreaker

#endif BRICKBREAKER_SCENE_SCENE_H_