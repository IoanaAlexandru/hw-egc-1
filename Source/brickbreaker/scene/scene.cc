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
            Scene::kBallToPlatformRatio = 0.15,
            Scene::kPauseButtonSize = 100;
const int Scene::kBricksPerRow = 15, Scene::kBrickRows = 8;

Scene::Scene() {}

Scene::~Scene() {}

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

  // CREATE PAUSE BUTTON

  glm::vec3 pause_button_corner =
      glm::vec3(resolution.x / 2 - kPauseButtonSize / 2,
                resolution.y / 2 + kPauseButtonSize / 2, 0);
  pause_button_ =
      new PauseButton("pausebutton", pause_button_corner, kPauseButtonSize);

  // CREATE WALLS
  float wall_thickness =
      std::min(scene_width_, scene_height_) * kWallThicknessRatio;
  glm::vec3 wall_color = glm::vec3(0.7, 0.2, 0.2);

  walls_.emplace(animatedmesh::UP,
                 new Wall("wall-up", animatedmesh::UP, scene_height_,
                          scene_width_, wall_thickness, wall_color));
  walls_.emplace(animatedmesh::LEFT,
                 new Wall("wall-left", animatedmesh::LEFT, scene_height_,
                          scene_width_, wall_thickness, wall_color));
  walls_.emplace(animatedmesh::RIGHT,
                 new Wall("wall-right", animatedmesh::RIGHT, scene_height_,
                          scene_width_, wall_thickness, wall_color));

  // CREATE BRICKS
  float brick_width = scene_width_ * kBrickPanelWidthRatio / kBricksPerRow *
                      (1 - kBrickDistanceRatio);
  float brick_height = scene_height_ * kBrickPanelHeightRatio / kBrickRows *
                       (1 - kBrickDistanceRatio);

  // Horizontal distance between bricks
  float brick_distance_x =
      (scene_width_ * kBrickPanelWidthRatio - kBricksPerRow * brick_width) /
      (kBricksPerRow - 1);
  // Vertical distance between bricks
  float brick_distance_y =
      (scene_height_ * kBrickPanelHeightRatio - kBrickRows * brick_height) /
      (kBrickRows - 1);

  // There is a row of free space between the brick panel and the upper wall, of
  // height equal to the brick height
  float x_offset = scene_width_ * (1 - kBrickPanelWidthRatio) / 2;
  // The brick panel is centered
  float y_offset = scene_height_ - (wall_thickness + brick_height);

  // Top left corner of first brick at the top left of the brick panel
  glm::vec3 brick_corner = glm::vec3(x_offset, y_offset, 0);

  glm::vec3 brick_color = glm::vec3(0.7, 0.2, 0.2);
  bool fill = true;

  for (auto i = 0; i < kBrickRows; i++) {
    std::vector<Brick*> brick_line;
    for (auto j = 0; j < kBricksPerRow; j++) {
      std::string name = "brick-" + std::to_string(i) + "-" + std::to_string(j);
      brick_line.push_back(new Brick(name, brick_corner, brick_height,
                                     brick_width, brick_color, fill));
      brick_corner += glm::vec3(brick_width + brick_distance_x, 0, 0);
    }
    bricks_.push_back(brick_line);
    brick_corner -= glm::vec3(0, brick_height + brick_distance_y, 0);
    brick_corner.x = x_offset;
  }

  // CREATE PLATFORM
  float platform_width = scene_width_ * kPlatformWidthRatio;
  float platform_height = platform_width * kPlatformHeightToWidthRatio;
  glm::vec3 platform_corner = glm::vec3((scene_width_ - platform_width) / 2,
                                        wall_thickness + platform_height, 0);
  glm::vec3 platform_color = glm::vec3(0.9, 0.4, 0.4);

  platform_ = new Platform("platform", platform_corner, platform_height,
                           platform_width, platform_color, fill);

  // CREATE BALL
  float ball_radius = platform_width * kBallToPlatformRatio / 2;
  glm::vec3 ball_center =
      platform_corner + glm::vec3(platform_width / 2, ball_radius, 0);
  glm::vec3 ball_color = glm::vec3(1, 1, 1);

  balls_.push_back(new Ball("ball-0", ball_center, ball_radius, ball_color));
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

  // RENDER BRICKS
  for (auto brick_line : bricks_) {
    for (auto brick : brick_line) {
      brick->Update(deltaTimeSeconds);
      RenderMesh2D(brick, shaders["VertexColor"], brick->GetModelMatrix());
    }
  }

  // RENDER WALLS
  for (auto wall : walls_) {
    RenderMesh2D(wall.second, shaders["VertexColor"], glm::mat3(1));
  }

  // RENDER PLATFORM
  platform_->Update(deltaTimeSeconds);
  RenderMesh2D(platform_, shaders["VertexColor"], platform_->GetModelMatrix());

  // CHECK BALL(S) COLLISIONS
  for (auto ball : balls_) {
    glm::vec3 ball_center = ball->GetCenter();
    float ball_radius = ball->GetRadius();

    // CHECK PLATFORM COLLISIONS
    if (ball_center.y < walls_[animatedmesh::UP]->GetThickness() +
                            platform_->GetHeight() + ball_radius) {
      ball->OnPlatformHit(platform_->GetCenter(), platform_->GetWidth());
      continue;
    }

    // CHECK WALL COLLISIONS
    for (auto wall : walls_) {
      float wall_thickness = wall.second->GetThickness();
      switch (wall.first) {
        case animatedmesh::UP:
          if (ball_center.y > scene_height_ - wall_thickness - ball_radius) {
            ball->OnHit(animatedmesh::UP);
          }
          break;
        case animatedmesh::DOWN:
          if (ball_center.y < wall_thickness + ball_radius) {
            ball->OnHit(animatedmesh::DOWN);
          }
          break;
        case animatedmesh::LEFT:
          if (ball_center.x < wall_thickness + ball_radius) {
            ball->OnHit(animatedmesh::LEFT);
          }
          break;
        case animatedmesh::RIGHT:
          if (ball_center.x > scene_width_ - wall_thickness - ball_radius) {
            ball->OnHit(animatedmesh::RIGHT);
          }
          break;
      }
    }

    // CHECK BRICK COLLISIONS
    for (auto brick_line : bricks_) {
      for (auto brick : brick_line) {
        float brick_height = brick->GetHeight(),
              brick_width = brick->GetWidth();
        glm::vec3 brick_center = brick->GetCenter();
        float up = brick_center.y + brick_height / 2,
              down = brick_center.y - brick_height / 2,
              left = brick_center.x - brick_width / 2,
              right = brick_center.x + brick_width / 2;
        glm::vec3 centers_difference = brick_center - ball_center;
        bool brick_is_solid = brick->IsSolid();

        if (abs(centers_difference.y) < brick_height / 2 + ball_radius &&
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
        } else if (abs(centers_difference.x) < brick_width / 2 + ball_radius &&
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

  // RENDER BALL(S)
  for (auto ball : balls_) {
    ball->Update(deltaTimeSeconds);
    RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
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
  float platform_width = platform_->GetWidth();
  float walls_thickness = walls_[animatedmesh::UP]->GetThickness();

  // Check left limit (platform stops at wall)
  new_pos = std::max(platform_width / 2 + walls_thickness, new_pos);
  // Check right limit
  new_pos =
      std::min(scene_width_ - platform_width / 2 - walls_thickness, new_pos);
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
