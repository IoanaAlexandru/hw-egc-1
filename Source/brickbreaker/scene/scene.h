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

  std::default_random_engine random_;
  std::bernoulli_distribution powerup_chance_;
  std::bernoulli_distribution powerup_spawn_chance_;
  inline bool ShouldSpawnPowerup() { return powerup_spawn_chance_(random_); }
  inline bool RandomPowerup() { return powerup_chance_(random_); }
  void SpawnPowerup(glm::vec3 top_left_corner);

  inline void ShrinkPlatform() {
    platform_->ShrinkWidth();
    platform_width_ = platform_->GetWidth();
  }

  inline void StretchPlatform() {
    platform_->StretchWidth();
    platform_width_ = platform_->GetWidth();
  }

  inline void AddBottomWall() {
    walls_.emplace(animatedmesh::DOWN,
                   new Wall("wall-down", animatedmesh::DOWN, scene_height_,
                            scene_width_, wall_thickness_, wall_color_));
  }

  inline void AddBall() {
    glm::vec3 ball_center = glm::vec3(
        scene_width_ / 2, wall_thickness_ + platform_height_ + ball_radius_, 0);
    std::string name = "ball-" + std::to_string(balls_.size());

    balls_.push_back(
        new Ball(name, ball_center, ball_radius_, ball_color_));
  }

  inline void DoNothing() {}

  inline void RemoveBottomWall() { walls_.erase(animatedmesh::DOWN); }

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
  static const float kPowerupSpawnChance;
  // chance for a certain type of powerup to spawn
  static const float kPowerupChance;
  static const float kPowerupSize;

  std::vector<std::vector<Brick *>> bricks_;
  std::map<animatedmesh::Position, Wall *> walls_;
  Platform *platform_;
  std::vector<Ball *> balls_;
  PauseButton *pause_button_;

  // First element is a powerup, second element is a pair of functions for
  // activating/deactivating the powerup's effect
  std::vector<
      std::pair<Powerup *, std::pair<void (Scene::*)(), void (Scene::*)()>>>
      powerups_;

  float scene_width_, scene_height_;
  float wall_thickness_;
  float brick_width_, brick_height_;
  float platform_width_, platform_height_;
  float ball_radius_;

  bool paused_ = false;
  int lives_ = kMaxLives;

  static const glm::vec3 wall_color_;
  static const glm::vec3 ball_color_;
};
}  // namespace brickbreaker

#endif BRICKBREAKER_SCENE_SCENE_H_
