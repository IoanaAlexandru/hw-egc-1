#include "brickbreaker/scene/scene.h"

#include <iostream>
#include <vector>

#include "Core/Engine.h"
#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
const float Scene::kBrickPanelWidthRatio = 0.8f,
            Scene::kBrickPanelHeightRatio = 0.5f,
            Scene::kPlatformWidthRatio = 0.15f,
            Scene::kPlatformHeightToWidthRatio = 0.1f,
            Scene::kWallThicknessRatio = 0.01f,
            Scene::kBrickDistanceRatio = 0.15f,
            Scene::kBallToPlatformRatio = 0.15f,
            Scene::kPauseButtonSize = 100.0f, Scene::kPowerupSpawnChance = 0.2f,
            Scene::kPowerupChance = 0.3f, Scene::kPowerupSize = 20.0f,
            Scene::kLifeSize = 30.0f;
const int Scene::kBricksPerRow = 15, Scene::kBrickRows = 8,
          Scene::kMaxLives = 3;
const glm::vec3 Scene::wall_color_ = glm::vec3(0.7, 0.2, 0.2),
                Scene::ball_color_ = glm::vec3(1, 1, 1);

Scene::Scene() {}

Scene::~Scene() {}

#pragma region INITIALISATION UTILS

void Scene::InitPauseButton() {
  glm::vec3 pause_button_corner =
      glm::vec3(scene_width_ / 2 - kPauseButtonSize / 2,
                scene_height_ / 2 + kPauseButtonSize / 2, 0);
  pause_button_ =
      new PauseButton("pausebutton", pause_button_corner, kPauseButtonSize);
}

void Scene::InitLives() {
  lives_.clear();

  glm::vec3 center = glm::vec3(kLifeSize * 1.2, kLifeSize * 0.9, 0);

  for (int i = 0; i < kMaxLives; i++) {
    std::string name = "life-" + std::to_string(i);
    lives_.push_back(new Life(name, center, kLifeSize, glm::vec3(1, 1, 1)));
    center += glm::vec3(kLifeSize * 1.5, 0, 0);
  }
}

void Scene::InitWalls() {
  walls_.clear();

  walls_.emplace(animatedmesh::UP,
                 new Wall("wall-up", animatedmesh::UP, scene_height_,
                          scene_width_, wall_thickness_, wall_color_));
  walls_.emplace(animatedmesh::LEFT,
                 new Wall("wall-left", animatedmesh::LEFT, scene_height_,
                          scene_width_, wall_thickness_, wall_color_));
  walls_.emplace(animatedmesh::RIGHT,
                 new Wall("wall-right", animatedmesh::RIGHT, scene_height_,
                          scene_width_, wall_thickness_, wall_color_));
}

void Scene::InitBrickPanel() {
  bricks_.clear();

  // Horizontal distance between bricks
  float brick_distance_x =
      (scene_width_ * kBrickPanelWidthRatio - kBricksPerRow * brick_width_) /
      (kBricksPerRow - 1);
  // Vertical distance between bricks
  float brick_distance_y =
      (scene_height_ * kBrickPanelHeightRatio - kBrickRows * brick_height_) /
      (kBrickRows - 1);

  // There is a row of free space between the brick panel and the upper wall,
  // of height equal to the brick height
  float x_offset = scene_width_ * (1 - kBrickPanelWidthRatio) / 2;
  // The brick panel is centered
  float y_offset = scene_height_ - (wall_thickness_ + brick_height_);

  // Top left corner of first brick at the top left of the brick panel
  glm::vec3 brick_corner = glm::vec3(x_offset, y_offset, 0);

  glm::vec3 brick_color = glm::vec3(0.7, 0.2, 0.2);
  bool fill = true;

  for (auto i = 0; i < kBrickRows; i++) {
    std::vector<Brick *> brick_line;
    for (auto j = 0; j < kBricksPerRow; j++) {
      std::string name = "brick-" + std::to_string(i) + "-" + std::to_string(j);
      brick_line.push_back(new Brick(name, brick_corner, brick_height_,
                                     brick_width_, brick_color, fill));
      brick_corner += glm::vec3(brick_width_ + brick_distance_x, 0, 0);
    }
    bricks_.push_back(brick_line);
    brick_corner -= glm::vec3(0, brick_height_ + brick_distance_y, 0);
    brick_corner.x = x_offset;
  }
}

void Scene::InitPlatform() {
  glm::vec3 platform_color = glm::vec3(0.9, 0.4, 0.4);
  glm::vec3 platform_corner = glm::vec3((scene_width_ - platform_width_) / 2,
                                        wall_thickness_ + platform_height_, 0);

  platform_ = new Platform("platform", platform_corner, platform_height_,
                           platform_width_, platform_color, true);
}

void Scene::InitBall() {
  balls_.clear();

  glm::vec3 ball_center = platform_->GetCenter() +
                          glm::vec3(0, ball_radius_ + platform_height_ / 2, 0);

  balls_.push_back(new Ball("ball-0", ball_center, ball_radius_, ball_color_));
}

#pragma endregion

void Scene::Init() {
  glm::ivec2 resolution = window->GetResolution();
  auto camera = GetSceneCamera();
  camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f,
                          400);
  camera->SetPosition(glm::vec3(0, 0, 50));
  camera->SetRotation(glm::vec3(0, 0, 0));
  camera->Update();
  GetCameraInput()->SetActive(false);

  powerup_spawn_chance_ = std::bernoulli_distribution(kPowerupSpawnChance);
  powerup_chance_ = std::bernoulli_distribution(kPowerupChance);

  scene_width_ = (float)resolution.x;
  scene_height_ = (float)resolution.y;

  wall_thickness_ = std::min(scene_width_, scene_height_) * kWallThicknessRatio;

  brick_width_ = scene_width_ * kBrickPanelWidthRatio / kBricksPerRow *
                 (1 - kBrickDistanceRatio);
  brick_height_ = scene_height_ * kBrickPanelHeightRatio / kBrickRows *
                  (1 - kBrickDistanceRatio);

  platform_width_ = scene_width_ * kPlatformWidthRatio;
  platform_height_ = platform_width_ * kPlatformHeightToWidthRatio;
  ball_radius_ = platform_width_ * kBallToPlatformRatio / 2;

  InitLives();
  InitPauseButton();
  InitWalls();
  InitBrickPanel();
  InitPlatform();
  InitBall();
}

void Scene::FrameStart() {
  // clears the color buffer (using the previously set color) and depth
  // buffer
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::ivec2 resolution = window->GetResolution();
  // sets the screen area where to draw
  glViewport(0, 0, resolution.x, resolution.y);
}

#pragma region COLLISION CHECKS

bool Scene::CheckCollision(
    Ball *ball, std::pair<const animatedmesh::Position, Wall *> wall) {
  glm::vec3 ball_center = ball->GetCenter();
  float ball_radius = ball->GetRadius();

  switch (wall.first) {
    case animatedmesh::UP:
      if (ball_center.y > scene_height_ - wall_thickness_ - ball_radius) {
        ball->OnHit(animatedmesh::UP);
        return true;
      }
      break;
    case animatedmesh::DOWN:
      if (ball_center.y < wall_thickness_ + ball_radius) {
        ball->OnHit(animatedmesh::DOWN);
        return true;
      }
      break;
    case animatedmesh::LEFT:
      if (ball_center.x < wall_thickness_ + ball_radius) {
        ball->OnHit(animatedmesh::LEFT);
        return true;
      }
      break;
    case animatedmesh::RIGHT:
      if (ball_center.x > scene_width_ - wall_thickness_ - ball_radius) {
        ball->OnHit(animatedmesh::RIGHT);
        return true;
      }
      break;
  }

  return false;
}

bool Scene::CheckCollision(brickbreaker::Ball *ball, Platform *platform) {
  if (ball->GetCenter().y <
      wall_thickness_ + platform->GetHeight() + ball->GetRadius()) {
    ball->OnPlatformHit(platform->GetCenter(), platform->GetWidth());
    return true;
  }

  return false;
}

bool Scene::CheckCollision(
    std::pair<Powerup *, std::pair<void (Scene::*)(), void (Scene::*)()>>
        powerup_and_effect,
    Platform *platform) {
  auto powerup = powerup_and_effect.first;
  auto activate = powerup_and_effect.second.first;

  if (powerup->GetCenter().y < wall_thickness_ + platform->GetHeight() &&
      !powerup->IsActivated()) {
    powerup->OnPlatformHit(platform->GetCenter(), platform->GetWidth());
    if (powerup->IsActivated()) (this->*activate)();
    return true;
  }

  return false;
}

bool Scene::CheckCollision(Ball *ball, Brick *brick) {
  glm::vec3 ball_center = ball->GetCenter();
  float ball_radius = ball->GetRadius();

  glm::vec3 brick_center = brick->GetCenter();
  float up = brick_center.y + brick_height_ / 2,
        down = brick_center.y - brick_height_ / 2,
        left = brick_center.x - brick_width_ / 2,
        right = brick_center.x + brick_width_ / 2;
  glm::vec3 centers_difference = brick_center - ball_center;
  bool brick_is_solid = brick->IsSolid();

  if (abs(centers_difference.y) < brick_height_ / 2 + ball_radius &&
      ball_center.x > left && ball_center.x < right) {
    if (centers_difference.y > 0) {
      brick->OnHit();
      if (brick->IsShrinking() && ShouldSpawnPowerup())
        SpawnPowerup(brick->GetCenter());
      if (brick_is_solid) {
        ball->OnHit(animatedmesh::UP);
        return true;
      }
    } else {
      brick->OnHit();
      if (brick->IsShrinking() && ShouldSpawnPowerup())
        SpawnPowerup(brick->GetCenter());
      if (brick_is_solid) {
        ball->OnHit(animatedmesh::DOWN);
        return true;
      }
    }
  } else if (abs(centers_difference.x) < brick_width_ / 2 + ball_radius &&
             ball_center.y < up && ball_center.y > down) {
    if (centers_difference.x > 0) {
      brick->OnHit();
      if (brick->IsShrinking() && ShouldSpawnPowerup())
        SpawnPowerup(brick->GetCenter());
      if (brick_is_solid) {
        ball->OnHit(animatedmesh::LEFT);
        return true;
      }
    } else {
      brick->OnHit();
      if (brick->IsShrinking() && ShouldSpawnPowerup())
        SpawnPowerup(brick->GetCenter());
      if (brick_is_solid) {
        ball->OnHit(animatedmesh::RIGHT);
        return true;
      }
    }
  }

  return false;
}

#pragma endregion

void Scene::SpawnPowerup(glm::vec3 top_left_corner) {
  glm::vec3 yellow = glm::vec3(0.96, 0.76, 0.05);
  glm::vec3 red = glm::vec3(0.86, 0.20, 0.21);
  glm::vec3 green = glm::vec3(0.24, 0.73, 0.33);

  Powerup *powerup;
  std::pair<void (Scene::*)(), void (Scene::*)()> effect;
  std::string name = "powerup-" + std::to_string(powerups_and_effects_.size());

  if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, red);
    effect = std::make_pair(&Scene::ShrinkPlatform, &Scene::StretchPlatform);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, green);
    effect = std::make_pair(&Scene::StretchPlatform, &Scene::ShrinkPlatform);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, green);
    effect = std::make_pair(&Scene::AddBottomWall, &Scene::RemoveBottomWall);
  } else {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, green);
    effect = std::make_pair(&Scene::AddBall, &Scene::DoNothing);
  }

  powerups_and_effects_.emplace_back(powerup, effect);
}

void Scene::Update(float delta_time_seconds) {
  // Only render the pause button if the game is paused
  if (paused_) {
    RenderMesh2D(pause_button_, shaders["VertexColor"], glm::mat3(1));
    return;
  }

  // Update and render lives
  for (auto life : lives_) {
    life->Update(delta_time_seconds);
    RenderMesh2D(life, shaders["VertexColor"], life->GetModelMatrix());
  }

  // Update and render bricks
  for (auto brick_line : bricks_) {
    for (auto brick : brick_line) {
      brick->Update(delta_time_seconds);
      RenderMesh2D(brick, shaders["VertexColor"], brick->GetModelMatrix());
    }
  }

  // Render walls
  for (auto wall : walls_) {
    RenderMesh2D(wall.second, shaders["VertexColor"], glm::mat3(1));
  }

  // Render platform
  platform_->Update(delta_time_seconds);
  RenderMesh2D(platform_, shaders["VertexColor"], platform_->GetModelMatrix());

  // Check powerup collisions
  for (auto p : powerups_and_effects_) {
    CheckCollision(p, platform_);
  }

  // Remove/deactivate fallen and expired powerups
  for (auto p = powerups_and_effects_.begin();
       p != powerups_and_effects_.end();) {
    auto powerup = (*p).first;
    auto deactivate = (*p).second.second;

    if (powerup->GetCenter().y < wall_thickness_ + platform_height_ &&
        !powerup->IsActivated()) {
      // powerup fell
      p = powerups_and_effects_.erase(p);
    } else if (powerup->IsActivated() && !powerup->IsActive()) {
      // powerup expired
      (this->*deactivate)();
      p = powerups_and_effects_.erase(p);
    } else {
      p++;
    }
  }

  // Render powerups
  for (auto p : powerups_and_effects_) {
    auto powerup = p.first;
    powerup->Update(delta_time_seconds);
    RenderMesh2D(powerup, shaders["VertexColor"], powerup->GetModelMatrix());
  }

  // Check ball collisions
  for (auto ball : balls_) {
    // Wall collisions
    for (auto wall : walls_) {
      if (CheckCollision(ball, wall)) continue;
    }

    // Platform collisions
    if (CheckCollision(ball, platform_)) continue;

    // Brick collisions
    int remaining_bricks = 0;  // bricks that haven't shrunk
    for (auto brick_line : bricks_) {
      for (auto brick : brick_line) {
        if (!brick->IsShrinking()) {
          if (CheckCollision(ball, brick)) continue;
          remaining_bricks++;
        }
      }
    }

    // Reset brick panel if no more bricks are left
    if (remaining_bricks == 0) {
      InitBrickPanel();
      return;
    }
  }

  // Remove fallen balls
  for (auto ball = balls_.begin(); ball != balls_.end();)
    if ((*ball)->GetCenter().y < 0)
      ball = balls_.erase(ball);
    else
      ball++;

  // Render balls
  for (auto ball : balls_) {
    ball->Update(delta_time_seconds);
    RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
  }

  // If no balls are left, remove a life and reset platform + ball
  if (balls_.size() == 0) {
    lives_.pop_back();
    InitPlatform();
    InitBall();
  }

  // If no lives are left, reset the game
  if (lives_.empty()) {
    InitBrickPanel();
    InitLives();
    powerups_and_effects_.clear();
  }
}

void Scene::FrameEnd() {}

void Scene::OnInputUpdate(float delta_time, int mods) {}

void Scene::OnKeyPress(int key, int mods) {
  if (key == GLFW_KEY_P) paused_ = !paused_;
}

void Scene::OnKeyRelease(int key, int mods) {}

void Scene::OnMouseMove(int mouse_x, int mouse_y, int delta_x, int delta_y) {
  // Don't go past left wall
  if (platform_->GetCenter().x < platform_width_ / 2 + wall_thickness_ &&
      delta_x < 0)
    return;

  // Don't go past right wall
  if (platform_->GetCenter().x >
          scene_width_ - platform_width_ / 2 - wall_thickness_ &&
      delta_x > 0)
    return;

  platform_->Move((float)delta_x);
  // If a ball is not moving (i.e., it is stuck to the platform), move it as
  // well, together with the platform.
  for (auto ball : balls_)
    if (!ball->IsMoving()) ball->Move((float)delta_x);
}

void Scene::OnMouseBtnPress(int mouse_x, int mouse_y, int button, int mods) {
  for (auto ball : balls_) {
    if (!ball->IsMoving()) {
      ball->StartMoving();
      break;
    }
  }
}

void Scene::OnMouseBtnRelease(int mouse_x, int mouse_y, int button, int mods) {}

void Scene::OnMouseScroll(int mouse_x, int mouse_y, int offset_x,
                          int offset_y) {}

void Scene::OnWindowResize(int width, int height) {}
}  // namespace brickbreaker
