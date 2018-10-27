#include "Scene.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Brick Breaker/Animations/Transform2D.h"

using namespace std;

const float Scene::BRICK_PANEL_WIDTH_RATIO = 0.75,
            Scene::BRICK_PANEL_HEIGHT_RATIO = 0.5,
            Scene::WALL_THICKNESS_RATIO = 0.01,
            Scene::BRICK_DISTANCE_RATIO = 0.1;
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

	float wall_thickness = min(resolution.x, resolution.y) * WALL_THICKNESS_RATIO;
	float brick_width = resolution.x * BRICK_PANEL_WIDTH_RATIO / BRICKS_PER_ROW * (1 - BRICK_DISTANCE_RATIO),
	      brick_height = resolution.y * BRICK_PANEL_HEIGHT_RATIO / BRICK_ROWS * (1 - BRICK_DISTANCE_RATIO);
	float brick_distance_x = (resolution.x * BRICK_PANEL_WIDTH_RATIO - BRICKS_PER_ROW * brick_width) / (BRICKS_PER_ROW - 1),
		  brick_distance_y = (resolution.y * BRICK_PANEL_HEIGHT_RATIO - BRICK_ROWS * brick_height) / (BRICK_ROWS - 1);
	float x_offset = resolution.x * (1 - BRICK_PANEL_WIDTH_RATIO) / 2,
		y_offset = resolution.y - (wall_thickness + brick_height);
	glm::vec3 corner = glm::vec3(x_offset, y_offset, 0),
		      color = glm::vec3(0.8, 0.3, 0.3);
	bool fill = true;

	for (auto i = 0; i < BRICK_ROWS; i++) {
		for (auto j = 0; j < BRICKS_PER_ROW; j++) {
			std::string name = "brick-" + to_string(i) + "-" + to_string(j);
			bricks.push_back(new Brick(name, corner, brick_height, brick_width, color, fill));
			corner += glm::vec3(brick_width + brick_distance_x, 0, 0);
		}
		corner -= glm::vec3(0, brick_height + brick_distance_y, 0);
		corner.x = x_offset;
	}

	walls.push_back(new Wall("wall-up", UP, resolution.y, resolution.x, wall_thickness, color));
	walls.push_back(new Wall("wall-left", LEFT, resolution.y, resolution.x, wall_thickness, color));
	walls.push_back(new Wall("wall-right", RIGHT, resolution.y, resolution.x, wall_thickness, color));
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
		RenderMesh2D(brick, shaders["VertexColor"], glm::mat3(1));
	}
	for (auto wall : walls) {
		RenderMesh2D(wall, shaders["VertexColor"], glm::mat3(1));
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
	// add mouse move event
}

void Scene::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
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
