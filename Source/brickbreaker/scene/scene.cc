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
            Scene::kBallToPlatformRatio = 0.15;
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
    for (auto j = 0; j < kBricksPerRow; j++) {
      std::string name = "brick-" + std::to_string(i) + "-" + std::to_string(j);
      bricks_.push_back(new Brick(name, brick_corner, brick_height, brick_width,
                                  brick_color, fill));
      brick_corner += glm::vec3(brick_width + brick_distance_x, 0, 0);
    }
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
  for (auto brick : bricks_) {
    brick->Update(deltaTimeSeconds);
    RenderMesh2D(brick, shaders["VertexColor"], brick->GetModelMatrix());
  }

  for (auto wall : walls_) {
    RenderMesh2D(wall.second, shaders["VertexColor"], glm::mat3(1));
  }

  platform_->Update(deltaTimeSeconds);
  RenderMesh2D(platform_, shaders["VertexColor"], platform_->GetModelMatrix());

  for (auto ball : balls_) {
    glm::vec3 center = ball->GetCenter();

    if (center.y < walls_[animatedmesh::UP]->GetThickness() +
                       platform_->GetHeight() + ball->GetRadius()) {
      ball->OnPlatformHit(platform_->GetTopLeftCorner(), platform_->GetWidth());
    }

    for (auto wall : walls_) {
      switch (wall.first) {
        case animatedmesh::UP:
          if (center.y >
              scene_height_ - wall.second->GetThickness() - ball->GetRadius())
            ball->OnWallCollision(animatedmesh::UP);
          break;
        case animatedmesh::DOWN:
          if (center.y < wall.second->GetThickness() + ball->GetRadius())
            ball->OnWallCollision(animatedmesh::DOWN);
          break;
        case animatedmesh::LEFT:
          if (center.x < wall.second->GetThickness() + ball->GetRadius())
            ball->OnWallCollision(animatedmesh::LEFT);
          break;
        case animatedmesh::RIGHT:
          if (center.x >
              scene_width_ - wall.second->GetThickness() - ball->GetRadius())
            ball->OnWallCollision(animatedmesh::RIGHT);
          break;
      }
    }

    ball->Update(deltaTimeSeconds);
    RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
  }
}

void Scene::FrameEnd() {}

void Scene::OnInputUpdate(float deltaTime, int mods) {}

void Scene::OnKeyPress(int key, int mods) {
  // add key press event
}

void Scene::OnKeyRelease(int key, int mods) {
  // add key release event
}

void Scene::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
  float new_pos = (float)mouseX;
  new_pos = std::max(
      platform_->GetWidth() / 2 + walls_[animatedmesh::UP]->GetThickness(),
      new_pos);  // left limit
  new_pos = std::min(scene_width_ - platform_->GetWidth() / 2 -
                         walls_[animatedmesh::UP]->GetThickness(),
                     new_pos);  // right limit
  new_pos -=
      scene_width_ / 2;  // take into account original position of platform

  platform_->Move(new_pos);
  for (auto ball : balls_) ball->Move(new_pos);
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
