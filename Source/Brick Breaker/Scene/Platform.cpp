#include "Platform.h"
#include "Brick Breaker/Generic/Transform2D.h"

Platform::Platform(std::string name, glm::vec3 top_left_corner, float height, float width, glm::vec3 color, bool fill)
	: Rectangle(name, top_left_corner, height, width, color, fill)
{
}

Platform::~Platform()
{
}

void Platform::Update(float delta_time_seconds) {
}

/*
Move platform center with new_position. (Platform only moves horizontally)
new_position > 0 => movement to the right
new_position < 0 => movement to the left
*/
void Platform::Move(float new_position) {
	top_left_corner_.x = initial_top_left_corner_.x + new_position;
	model_matrix_ = Transform2D::Translate(new_position, 0);
}