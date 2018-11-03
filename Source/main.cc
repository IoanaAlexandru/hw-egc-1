#include <ctime>
#include <iostream>

#include "Core/Engine.h"
#include "brickbreaker/scene/scene.h"

int main(int argc, char **argv) {
  srand((unsigned int)time(NULL));

  // Create a window property structure
  WindowProperties wp;
  wp.name = "Brick Breaker";
  wp.resolution = glm::ivec2(1280, 720);

  // Init the Engine and create a new window with the defined properties
  WindowObject *window = Engine::Init(wp);

  // Create game scene
  World *scene = new brickbreaker::Scene();
  scene->Init();
  scene->Run();

  // Signals to the Engine to release the OpenGL context
  Engine::Exit();

  return 0;
}