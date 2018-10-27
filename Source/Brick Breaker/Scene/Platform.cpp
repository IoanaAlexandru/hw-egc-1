#include "Platform.h"
#include "Brick Breaker/Generic/Transform2D.h"

Platform::Platform(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill)
	: Rectangle(name, topLeftCorner, height, width, color, fill)
{
}

Platform::~Platform()
{
}

void Platform::Update(float delta_time_seconds) {
}

void Platform::Move(float new_position) {
	model_matrix_ = Transform2D::Translate(new_position, 0);
}