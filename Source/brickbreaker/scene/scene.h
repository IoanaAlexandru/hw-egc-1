#ifndef BRICKBREAKER_SCENE_SCENE_H_
#define BRICKBREAKER_SCENE_SCENE_H_

#pragma once

#include <map>
#include <random>
#include <string>

#include "Component/SimpleScene.h"
#include "Core/Engine.h"
#include "brickbreaker/scene/game/ball.h"
#include "brickbreaker/scene/game/brick.h"
#include "brickbreaker/scene/game/platform.h"
#include "brickbreaker/scene/game/powerup.h"
#include "brickbreaker/scene/game/wall.h"
#include "brickbreaker/scene/ui/life.h"
#include "brickbreaker/scene/ui/pausebutton.h"

namespace brickbreaker {
class Scene : public SimpleScene {
 public:
  Scene();
  ~Scene();

  void Init() override;

 private:
  // INHERITED METHODS

  void FrameStart() override;
  void Update(float delta_time_seconds) override;
  void FrameEnd() override;

  void OnInputUpdate(float delta_time, int mods) override;
  void OnKeyPress(int key, int mods) override;
  void OnKeyRelease(int key, int mods) override;
  void OnMouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) override;
  void OnMouseBtnPress(int mouse_x, int mouse_y, int button, int mods) override;
  void OnMouseBtnRelease(int mouse_x, int mouse_y, int button,
                         int mods) override;
  void OnMouseScroll(int mouse_x, int mouse_y, int offset_x,
                     int offset_y) override;
  void OnWindowResize(int width, int height) override;

  // INITIALIZATION

  void InitPauseButton();
  void InitLives();
  void InitWalls();
  void InitBrickPanel();
  void InitPlatform();
  void InitBall();

  // RANDOMIZATION

  // randomization engine
  std::default_random_engine random_;
  // chance of a specific power-up to spawn
  std::bernoulli_distribution powerup_chance_;
  // chance of power-up to spawn after a brick is destroyed
  std::bernoulli_distribution powerup_spawn_chance_;
  // chance of a solid brick to be initialised
  std::bernoulli_distribution solid_brick_chance_;
  inline bool ShouldSpawnPowerup() { return powerup_spawn_chance_(random_); }
  inline bool RandomPowerup() { return powerup_chance_(random_); }
  inline bool SolidBrick() { return solid_brick_chance_(random_); }
  void SpawnPowerup(glm::vec3 top_left_corner);

  // COLLISIONS

  // Check and act upon collision between ball and wall
  bool CheckCollision(
      Ball *ball,
      std::pair<const animatedmesh::Position, brickbreaker::Wall *> wall);
  // Check and act upon collision between ball and platform
  bool CheckCollision(Ball *ball, Platform *platform);
  // Check and act upon collision between power-up and platform
  bool CheckCollision(
      std::pair<Powerup *, std::pair<void (Scene::*)(), void (Scene::*)()>>
          powerup_and_effect,
      Platform *platform);
  // Act upon collision between power-up and brick based on the position
  bool Collide(Ball *ball, Brick *brick, animatedmesh::Position position);
  // Check collision between ball and brick
  bool CheckCollision(Ball *ball, Brick *brick);

  // POWERUP EFFECTS

  // Make platform width smaller
  inline void ShrinkPlatform() {
    platform_->ShrinkWidth();
    platform_width_ = platform_->GetWidth();
  }

  // Make platform width larger
  inline void StretchPlatform() {
    platform_->StretchWidth();
    platform_width_ = platform_->GetWidth();
  }

  // Create wall at the bottom of the scene
  inline void AddBottomWall() {
    walls_.emplace(animatedmesh::DOWN,
                   new Wall("wall-down", animatedmesh::DOWN, scene_height_,
                            scene_width_, wall_thickness_, wall_color_));
  }

  // Remove wall at the bottom of the scene
  inline void RemoveBottomWall() { walls_.erase(animatedmesh::DOWN); }

  // Create new ball and put it on the platform
  inline void AddBall() {
    glm::vec3 ball_center =
        platform_->GetCenter() +
        glm::vec3(0, ball_radius_ + platform_height_ / 2, 0);
    std::string name = "ball-" + std::to_string(balls_.size());

    balls_.push_back(new Ball(name, ball_center, ball_radius_, ball_color_));
  }

  // Used as a disable trigger for powerups that don't expire
  inline void DoNothing() {}

  // Speed up all balls
  inline void SpeedUpBalls() {
    for (auto ball : balls_) ball->SpeedUp();
  }

  // Speed down all balls
  inline void SlowDownBalls() {
    for (auto ball : balls_) ball->SlowDown();
  }

  // Make ball stick to the platform on contact (needs a click for release)
  inline void MakePlatformSticky() { platform_->MakeSticky(); }

  // Undo sticky platform
  inline void MakePlatformNotSticky() { platform_->MakeNotSticky(); }

  // Make balls pass through walls
  inline void PowerUpBalls() {
    for (auto ball : balls_) ball->PowerUp();
  }

  // Undo make balls pass through walls
  inline void PowerDownBalls() {
    for (auto ball : balls_) ball->PowerDown();
  }

  // CONSTANTS

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
  static const float kLifeSize;
  static const float kLifeSpaceSize;
  // chance of a brick to be solid
  static const float kSolidBrickChance;

  // SCENE ELEMENTS

  std::vector<std::vector<Brick *>> bricks_;
  std::map<animatedmesh::Position, Wall *> walls_;
  Platform *platform_;
  std::vector<Ball *> balls_;
  std::vector<Life *> lives_;
  PauseButton *pause_button_;
  // First element is a powerup, second element is a pair of functions for
  // activating/deactivating the powerup's effect
  std::vector<
      std::pair<Powerup *, std::pair<void (Scene::*)(), void (Scene::*)()>>>
      powerups_and_effects_;

  // ELEMENT PROPERTIES

  float scene_width_, scene_height_;
  float wall_thickness_;
  float brick_width_, brick_height_;
  float platform_width_, platform_height_;
  float ball_radius_;

  // SCENE PROPERTIES

  bool paused_ = false;

  // COLORS

  glm::vec3 wall_color_ = glm::vec3(0.7, 0.2, 0.2);
  glm::vec3 ball_color_ = kWhite;
  glm::vec3 life_color_ = kRed;
  glm::vec3 brick_color_ = glm::vec3(0.7, 0.2, 0.2);
  glm::vec3 platform_color_ = glm::vec3(0.9, 0.4, 0.4);
};
}  // namespace brickbreaker

#endif BRICKBREAKER_SCENE_SCENE_H_
