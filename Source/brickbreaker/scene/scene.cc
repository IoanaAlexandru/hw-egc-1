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
            Scene::kPauseButtonSize = 100.0f, Scene::kPowerupSpawnChance = 0.3f,
            Scene::kPowerupChance = 0.2f, Scene::kPowerupSize = 20.0f,
            Scene::kLifeSize = 30.0f, Scene::kLifeSpaceSize = 10.0f,
            Scene::kSolidBrickChance = 0.8f;

const int Scene::kBricksPerRow = 15, Scene::kBrickRows = 8,
          Scene::kMaxLives = 3;

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

  glm::vec3 center =
      glm::vec3(kLifeSize / 2 + kLifeSpaceSize + wall_thickness_,
                kLifeSize / 2 + kLifeSpaceSize + wall_thickness_, 0);

  for (int i = 0; i < kMaxLives; i++) {
    std::string name = "life-" + std::to_string(i);
    lives_.push_back(new Life(name, center, kLifeSize, life_color_));
    center += glm::vec3(kLifeSize + kLifeSpaceSize, 0, 0);
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

  for (auto i = 0; i < kBrickRows; i++) {
    std::vector<Brick *> brick_line;
    for (auto j = 0; j < kBricksPerRow; j++) {
      bool fill = SolidBrick();
      std::string name = "brick-" + std::to_string(i) + "-" + std::to_string(j);
      brick_line.push_back(new Brick(name, brick_corner, brick_height_,
                                     brick_width_, brick_color_, fill));
      brick_corner += glm::vec3(brick_width_ + brick_distance_x, 0, 0);
    }
    bricks_.push_back(brick_line);
    brick_corner -= glm::vec3(0, brick_height_ + brick_distance_y, 0);
    brick_corner.x = x_offset;
  }
}

void Scene::InitPlatform() {
  glm::vec3 platform_corner = glm::vec3((scene_width_ - platform_width_) / 2,
                                        wall_thickness_ + platform_height_, 0);

  platform_ = new Platform("platform", platform_corner, platform_height_,
                           platform_width_, platform_color_, true);
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
  solid_brick_chance_ = std::bernoulli_distribution(kSolidBrickChance);

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
  // Value indicating how much into the wall the ball is; used to prevent the
  // case when the platform would push the ball so far into the lateral wall
  // it would get stuck there
  float penetration;

  switch (wall.first) {
    case animatedmesh::UP:
      if (ball_center.y >= scene_height_ - wall_thickness_ - ball_radius) {
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
      penetration = ball_center.x - (wall_thickness_ + ball_radius);
      if (penetration <= 0) {
        ball->Move(-penetration);
        ball->OnHit(animatedmesh::LEFT);
        return true;
      }
      break;
    case animatedmesh::RIGHT:
      penetration =
          ball_center.x - (scene_width_ - wall_thickness_ - ball_radius);
      if (penetration >= 0) {
        ball->Move(-penetration);
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
    ball->OnPlatformHit(platform->GetCenter(), platform->GetWidth(),
                        platform->GetHeight(), platform->IsSticky());
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

bool Scene::Collide(Ball *ball, Brick *brick,
                    animatedmesh::Position brick_position) {
  bool brick_is_solid = brick->IsSolid() && !ball->IsPoweredUp();

  brick->OnHit();
  if (brick->IsShrinking() && ShouldSpawnPowerup())
    SpawnPowerup(brick->GetCenter());
  if (brick_is_solid) {
    ball->OnHit(brick_position);
    return true;
  }

  return false;
}

float GetEuclideanDistance(glm::vec3 distance) {
  return sqrt(pow(distance.x, 2) + pow(distance.y, 2));
}

bool Scene::CheckCollision(Ball *ball, Brick *brick) {
  glm::vec3 ball_center = ball->GetCenter();

  glm::vec3 brick_center = brick->GetCenter();
  // distance (x, y) between the center of the ball and  the center of the brick
  glm::vec3 centers_distance = brick_center - ball_center;
  glm::vec3 abs_centers_distance = abs(centers_distance);
  // distance between the center of the ball and the corner of the brick
  glm::vec3 corner_distance =
      glm::vec3(abs_centers_distance.x - brick_width_ / 2,
                abs_centers_distance.y - brick_height_ / 2, 0);

  // eliminate cases when the ball is too far to be touching the brick
  if (abs_centers_distance.x > brick_width_ / 2 + ball_radius_) return false;
  if (abs_centers_distance.y > brick_height_ / 2 + ball_radius_) return false;

  // straight collisions
  if (abs_centers_distance.y <= brick_height_ / 2)
    if (centers_distance.x > 0) {
      if (Collide(ball, brick, animatedmesh::LEFT)) return true;
    } else {
      if (Collide(ball, brick, animatedmesh::RIGHT)) return true;
    }
  if (abs_centers_distance.x <= brick_width_ / 2)
    if (centers_distance.y > 0) {
      if (Collide(ball, brick, animatedmesh::UP)) return true;
    } else {
      if (Collide(ball, brick, animatedmesh::DOWN)) return true;
    }

  // corner collisions
  if (GetEuclideanDistance(corner_distance) <= ball_radius_)
    if (corner_distance.x < corner_distance.y)
      if (centers_distance.x > 0) {
        if (Collide(ball, brick, animatedmesh::LEFT)) return true;
      } else {
        if (Collide(ball, brick, animatedmesh::RIGHT)) return true;
      }
    else if (centers_distance.y > 0) {
      if (Collide(ball, brick, animatedmesh::UP)) return true;
    } else {
      if (Collide(ball, brick, animatedmesh::DOWN)) return true;
    }

  return false;
}

#pragma endregion

void Scene::SpawnPowerup(glm::vec3 top_left_corner) {
  Powerup *powerup;
  std::pair<void (Scene::*)(), void (Scene::*)()> effect;
  std::string name = "powerup-" + std::to_string(powerups_and_effects_.size());

  if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, kRed);
    effect = std::make_pair(&Scene::ShrinkPlatform, &Scene::StretchPlatform);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, kGreen);
    effect = std::make_pair(&Scene::StretchPlatform, &Scene::ShrinkPlatform);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, wall_color_);
    effect = std::make_pair(&Scene::AddBottomWall, &Scene::RemoveBottomWall);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, platform_color_);
    effect = std::make_pair(&Scene::MakePlatformSticky,
                            &Scene::MakePlatformNotSticky);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, kPurple);
    effect = std::make_pair(&Scene::PowerUpBalls, &Scene::PowerDownBalls);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, kBlue);
    effect = std::make_pair(&Scene::SpeedUpBalls, &Scene::SlowDownBalls);
  } else if (RandomPowerup()) {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, kYellow);
    effect = std::make_pair(&Scene::SlowDownBalls, &Scene::SpeedUpBalls);
  } else {
    powerup = new Powerup(name, top_left_corner, kPowerupSize, ball_color_);
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

  // Render powerups
  for (auto p : powerups_and_effects_) {
    auto powerup = p.first;
    powerup->Update(delta_time_seconds);
    RenderMesh2D(powerup, shaders["VertexColor"], powerup->GetModelMatrix());
  }

  // Render balls
  for (auto ball : balls_) {
    ball->Update(delta_time_seconds);
    RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
  }

  // Check powerup collisions
  for (auto p : powerups_and_effects_) {
    CheckCollision(p, platform_);
  }

  // Remove/deactivate fallen and expired powerups
  for (auto p = powerups_and_effects_.begin();
       p != powerups_and_effects_.end();) {
    auto powerup = (*p).first;
    auto deactivate = (*p).second.second;

    if (powerup->GetCenter().y < 0 && !powerup->IsActivated()) {
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

  // Check ball collisions
  for (auto ball : balls_) {
    // Ball can only collide with one thing in one frame (to avoid conflicts)
    bool already_collided = false;

    // Wall collisions
    for (auto wall : walls_) {
      if (CheckCollision(ball, wall)) {
        already_collided = true;
        break;
      }
    }

    if (already_collided) continue;

    // Platform collisions
    if (CheckCollision(ball, platform_)) continue;

    // Brick collisions
    int remaining_bricks = 0;  // bricks that haven't shrunk
    for (auto brick_line : bricks_) {
      for (auto brick : brick_line) {
        if (!brick->IsShrinking()) {
          remaining_bricks++;
          if (CheckCollision(ball, brick)) {
            already_collided = true;
            break;
          }
        }
      }
      if (already_collided) break;
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
      ball->StartMoving(platform_->GetCenter(), platform_width_,
                        platform_height_);
      break;
    }
  }
}

void Scene::OnMouseBtnRelease(int mouse_x, int mouse_y, int button, int mods) {}

void Scene::OnMouseScroll(int mouse_x, int mouse_y, int offset_x,
                          int offset_y) {}

void Scene::OnWindowResize(int width, int height) {}
}  // namespace brickbreaker
