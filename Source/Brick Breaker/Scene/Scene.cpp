#include "Scene.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Brick Breaker/Generic/Transform2D.h"

using namespace std;

const float Scene::BRICK_PANEL_WIDTH_RATIO = 0.8,
Scene::BRICK_PANEL_HEIGHT_RATIO = 0.5,
Scene::PLATFORM_WIDTH_RATIO = 0.15,
Scene::PLATFORM_HEIGHT_TO_WIDTH_RATIO = 0.1,
Scene::WALL_THICKNESS_RATIO = 0.01,
Scene::BRICK_DISTANCE_RATIO = 0.15,
Scene::BALL_TO_PLATFORM_RATIO = 0.15;
const int Scene::BRICKS_PER_ROW = 15,
Scene::BRICK_ROWS = 8;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	scene_width = resolution.x;
	scene_height = resolution.y;

	// CREATE WALLS
	float wall_thickness = min(scene_width, scene_height) * WALL_THICKNESS_RATIO;
	glm::vec3 wall_color = glm::vec3(0.7, 0.2, 0.2);

	walls.emplace(UP, new Wall("wall-up", UP, scene_height, scene_width, wall_thickness, wall_color));
	walls.emplace(LEFT, new Wall("wall-left", LEFT, scene_height, scene_width, wall_thickness, wall_color));
	walls.emplace(RIGHT, new Wall("wall-right", RIGHT, scene_height, scene_width, wall_thickness, wall_color));

	// CREATE BRICKS
	float brick_width = scene_width * BRICK_PANEL_WIDTH_RATIO / BRICKS_PER_ROW * (1 - BRICK_DISTANCE_RATIO);
	float brick_height = scene_height * BRICK_PANEL_HEIGHT_RATIO / BRICK_ROWS * (1 - BRICK_DISTANCE_RATIO);

	// Horizontal distance between bricks
	float brick_distance_x = (scene_width * BRICK_PANEL_WIDTH_RATIO - BRICKS_PER_ROW * brick_width) / (BRICKS_PER_ROW - 1);
	// Vertical distance between bricks
	float brick_distance_y = (scene_height * BRICK_PANEL_HEIGHT_RATIO - BRICK_ROWS * brick_height) / (BRICK_ROWS - 1);

	// There is a row of free space between the brick panel and the upper wall, of height equal to the brick height
	float x_offset = scene_width * (1 - BRICK_PANEL_WIDTH_RATIO) / 2;
	// The brick panel is centered
	float y_offset = scene_height - (wall_thickness + brick_height);

	// Top left corner of first brick at the top left of the brick panel
	glm::vec3 brick_corner = glm::vec3(x_offset, y_offset, 0);

	glm::vec3 brick_color = glm::vec3(0.7, 0.2, 0.2);
	bool fill = true;

	for (auto i = 0; i < BRICK_ROWS; i++) {
		for (auto j = 0; j < BRICKS_PER_ROW; j++) {
			std::string name = "brick-" + to_string(i) + "-" + to_string(j);
			bricks.push_back(new Brick(name, brick_corner, brick_height, brick_width, brick_color, fill));
			brick_corner += glm::vec3(brick_width + brick_distance_x, 0, 0);
		}
		brick_corner -= glm::vec3(0, brick_height + brick_distance_y, 0);
		brick_corner.x = x_offset;
	}

	// CREATE PLATFORM
	float platform_width = scene_width * PLATFORM_WIDTH_RATIO;
	float platform_height = platform_width * PLATFORM_HEIGHT_TO_WIDTH_RATIO;
	glm::vec3 platform_corner = glm::vec3((scene_width - platform_width) / 2, wall_thickness + platform_height, 0);
	glm::vec3 platform_color = glm::vec3(0.9, 0.4, 0.4);

	platform = new Platform("platform", platform_corner, platform_height, platform_width, platform_color, fill);

	// CREATE BALL
	float ball_radius = platform_width * BALL_TO_PLATFORM_RATIO / 2;
	glm::vec3 ball_center = platform_corner + glm::vec3(platform_width / 2, ball_radius, 0);
	glm::vec3 ball_color = glm::vec3(1, 1, 1);
	
	balls.push_back(new Ball("ball-0", ball_center, ball_radius, ball_color));
}

void Scene::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Scene::Update(float deltaTimeSeconds)
{
	for (auto brick : bricks) {
		brick->Update(deltaTimeSeconds);
		RenderMesh2D(brick, shaders["VertexColor"], brick->GetModelMatrix());
	}

	for (auto wall : walls) {
		RenderMesh2D(wall.second, shaders["VertexColor"], glm::mat3(1));
	}

	platform->Update(deltaTimeSeconds);
	RenderMesh2D(platform, shaders["VertexColor"], platform->GetModelMatrix());

	for (auto ball : balls) {
		glm::vec3 center = ball->GetCenter();

		if (center.y < walls[UP]->GetThickness() + platform->GetHeight() + ball->GetRadius()) {
			ball->OnPlatformHit(platform->GetTopLeftCorner(), platform->GetWidth());
		}

		for (auto wall : walls) {
			switch (wall.first) {
			case UP:
				if (center.y > scene_height - wall.second->GetThickness() - ball->GetRadius())
					ball->OnWallCollision(UP);
				break;
			case DOWN:
				if (center.y < wall.second->GetThickness() + ball->GetRadius())
					ball->OnWallCollision(DOWN);
				break;
			case LEFT:
				if (center.x < wall.second->GetThickness() + ball->GetRadius())
					ball->OnWallCollision(LEFT);
				break;
			case RIGHT:
				if (center.x > scene_width - wall.second->GetThickness() - ball->GetRadius())
					ball->OnWallCollision(RIGHT);
				break;
			}
		}

		ball->Update(deltaTimeSeconds);
		RenderMesh2D(ball, shaders["VertexColor"], ball->GetModelMatrix());
	}
}

void Scene::FrameEnd()
{

}

void Scene::OnInputUpdate(float deltaTime, int mods)
{
	
}

void Scene::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Scene::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Scene::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	float new_pos = mouseX;
	new_pos = max(platform->GetWidth() / 2 + walls[UP]->GetThickness(), new_pos);  // left limit
	new_pos = min(scene_width - platform->GetWidth() / 2 - walls[UP]->GetThickness(), new_pos);  // right limit
	new_pos -= scene_width / 2;  // take into account original position of platform

	platform->Move(new_pos);
	for (auto ball : balls)
		ball->Move(new_pos);
}

void Scene::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	for (auto ball : balls) {
		ball->StartMoving();
	}
}

void Scene::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Scene::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Scene::OnWindowResize(int width, int height)
{
}
