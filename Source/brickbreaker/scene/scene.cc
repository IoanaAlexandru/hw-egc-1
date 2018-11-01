#include "brickbreaker/scene/scene.h"

#include <iostream>
#include <vector>

#include "Core/Engine.h"
#include "brickbreaker/generic/transform2d.h"

namespace brickbreaker {
const float Scene::kBrickPanelWidthRatio = 0.8,
            Scene::kBrickPanelHeightRatio = 0.5,
            Scene::kPlatformWidthRatio = 0.15,
            Scene::kPlatformHeightToWidthRatio = 0.1,
            Scene::kWallThicknessRatio = 0.01,
            Scene::kBrickDistanceRatio = 0.15,
            Scene::kBallToPlatformRatio = 0.15, Scene::kPauseButtonSize = 100;
const int Scene::kBricksPerRow = 15, Scene::kBrickRows = 8,
          Scene::kMaxLives = 3;

Scene::Scene() {}

Scene::~Scene() {}

void Scene::InitPauseButton() {
  glm::vec3 pause_button_corner =
      glm::vec3(scene_width_ / 2 - kPauseButtonSize / 2,
                scene_height_ / 2 + kPauseButtonSize / 2, 0);
  pause_button_ =
      new PauseButton("pausebutton", pause_button_corner, kPauseButtonSize);
}

void Scene::InitWalls() {
  walls_.clear();
  glm::vec3 wall_color = glm::vec3(0.7, 0.2, 0.2);

  walls_.emplace(animatedmesh::UP,
                 new Wall("wall-up", animatedmesh::UP, scene_height_,
                          scene_width_, wall_thickness_, wall_color));
  walls_.emplace(animatedmesh::LEFT,
                 new Wall("wall-left", animatedmesh::LEFT, scene_height_,
                          scene_width_, wall_thickness_, wall_color));
  walls_.emplace(animatedmesh::RIGHT,
                 new Wall("wall-right", animatedmesh::RIGHT, scene_height_,
                          scene_width_, wall_thickness_, wall_color));
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

  // There is a row of free space between the brick panel and the upper wall, of
  // height equal to the brick height
  float x_offset = scene_width_ * (1 - kBrickPanelWidthRatio) / 2;
  // The brick panel is centered
  float y_offset = scene_height_ - (wall_thickness_ + brick_height_);

  // Top left corner of first brick at the top left of the brick panel
  glm::vec3 brick_corner = glm::vec3(x_offset, y_offset, 0);

  glm::vec3 brick_color = glm::vec3(0.7, 0.2, 0.2);
  bool fill = true;

  for (auto i = 0; i < kBrickRows; i++) {
    std::vector<Brick*> brick_line;
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

  float ball_radius = platform_width_ * kBallToPlatformRatio / 2;
  glm::vec3 ball_center = glm::vec3(
      scene_width_ / 2, wall_thickness_ + platform_height_ + ball_radius, 0);
  glm::vec3 ball_color = glm::vec3(1, 1, 1);

  balls_.push_back(new Ball("ball-0", ball_center, ball_radius, ball_color));
}

void Scene::Init() {
  glm::ivec2 resolution = window->GetResolution();
  auto camera = GetSceneCamera();
  camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f,
                          400);
  camera->SetPosition(glm::vec3(0, 0, 50));
  camera->SetRotation(glm::vec3(0, 0, 0));
  camera->Update();
  GetCameraInput()->SetActive(false);

  scene_width_ = resolution.x;
  scene_height_ = resolution.y;

  wall_thickness_ = std::min(scene_width_, scene_height_) * kWallThicknessRatio;

  brick_width_ = scene_width_ * kBrickPanelWidthRatio / kBricksPerRow *
                 (1 - kBrickDistanceRatio);
  brick_height_ = scene_height_ * kBrickPanelHeightRatio / kBrickRows *
                  (1 - kBrickDistanceRatio);

  platform_width_ = scene_width_ * kPlatformWidthRatio;
  platform_height_ = platform_width_ * kPlatformHeightToWidthRatio;

  InitPauseButton();
  InitWalls();
  InitBrickPanel();
  InitPlatform();
  InitBall();
}

void Scene::FrameStart() {
  // clears the color buffer (using the previously set color) and depth buffer
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::ivec2 resolution = window->GetResolution();
  // sets the screen area where to draw
  glViewport(0, 0, resolution.x, resolution.y);
}

void Scene::Update(float deltaTimeSeconds) {
  if (paused_) {
    RenderMesh2D(pause_button_, shaders["VertexColor"], glm::mat3(1));
    return;
  }

  // Render bricks
  for (auto brick_line : bricks_) {
    for (auto brick : brick_line) {
      brick->Update(deltaTimeSeconds);
      RenderMesh2D(brick, shaders["VertexColor"], brick->GetModelMatrix());
    }
  }

  // Render walls
  for (auto wall : walls_) {
    RenderMesh2D(wall.second, shaders["VertexColor"], glm::mat3(1));
  }

  // Render platform
  platform_->Update(deltaTimeSeconds);
  RenderMesh2D(platform_, shaders["VertexColor"], platform_->GetModelMatrix());

  // Check ball(s) collisions
  for (auto ball : balls_) {
    glm::vec3 ball_center = ball->GetCenter();
    float ball_radius = ball->GetRadius();

    // Check platform collisions
    if (ball_center.y < wall_thickness_ + platform_height_ + ball_radius) {
      ball->OnPlatformHit(platform_->GetCenter(), platform_width_);
      continue;
    }

    // Check wall collisions
    for (auto wall : walls_) {
      switch (wall.first) {
        case animatedmesh::UP:
          if (ball_center.y > scene_height_ - wall_thickness_ - ball_radius) {
            ball->OnHit(animatedmesh::UP);
          }
          break;
        case animatedmesh::DOWN:
          if (ball_center.y < wall_thickness_ + ball_radius) {
            ball->OnHit(animatedmesh::DOWN);
          }
          break;
        case animatedmesh::LEFT:
          if (ball_center.x < wall_thickness_ + ball_radius) {
            ball->OnHit(animatedmesh::LEFT);
          }
          break;
        case animatedmesh::RIGHT:
          if (ball_center.x > scene_width_ - wall_thickness_ - ball_radius) {
            ball->OnHit(animatedmesh::RIGHT);
          }
          break;
      }
    }

    // Check brick collisions
    for (auto brick_line : bricks_) {
      for (auto brick : brick_line) {
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
            if (brick_is_solid) {
              ball->OnHit(animatedmesh::UP);
              continue;
            }
          } else {
            brick->OnHit();
            if (brick_is_solid) {
              ball->OnHit(animatedmesh::DOWN);
              continue;
            }
          }
        } else if (abs(centers_difference.x) < brick_width_ / 2 + ball_radius &&
                   ball_center.y < up && ball_center.y > down) {
          if (centers_difference.x > 0) {
            brick->OnHit();
            if (brick_is_solid) {
              ball->OnHit(animatedmesh::LEFT);
              continue;
            }
          } else {
            brick->OnHit();
            if (brick_is_solid) {
              ball->OnHit(animatedmesh::RIGHT);
              continue;
            }
          }
        }
      }
    }
  }

  // Remove fallen balls
  for (auto ball = balls_.begin(); ball != balls_.end();)
    if ((*ball)->GetCenter().y < 0)
      ball = balls_.erase(ball);
    else
      ball++;

  // Render ball(s)
  for (auto ball : balls_) {
    ball->Update(deltaTimeSeconds);
    RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
  }

  // If no balls are left, subtract a life and reset platform + ball
  if (balls_.size() == 0) {
    lives_--;
    InitPlatform();
    InitBall();
  }

  // If no lives are left, reset brick panel and lives count
  if (lives_ <= 0) {
    InitBrickPanel();
    lives_ = kMaxLives;
  }
}

void Scene::FrameEnd() {}

void Scene::OnInputUpdate(float deltaTime, int mods) {}

void Scene::OnKeyPress(int key, int mods) {
  if (key == GLFW_KEY_P) paused_ = !paused_;
}

void Scene::OnKeyRelease(int key, int mods) {
  // add key release event
}

void Scene::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
  float new_pos = (float)mouseX;

  // Check left limit (platform stops at wall)
  new_pos = std::max(platform_width_ / 2 + wall_thickness_, new_pos);
  // Check right limit
  new_pos =
      std::min(scene_width_ - platform_width_ / 2 - wall_thickness_, new_pos);
  // Take into account original position of platform
  new_pos -= scene_width_ / 2;

  platform_->Move(new_pos);
  // If a ball is not moving (i.e., it is stuck to the platform), move it as
  // well, together with the platform.
  for (auto ball : balls_)
    if (!ball->IsMoving()) ball->Move(new_pos);
}

void Scene::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
  for (auto ball : balls_) {
    ball->StartMoving();
  }
}

void Scene::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
  // add mouse button release event
}

void Scene::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Scene::OnWindowResize(int width, int height) {}
}  // namespace brickbreaker
