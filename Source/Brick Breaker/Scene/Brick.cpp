#include "Brick.h"

Brick::Brick(std::string name, glm::vec3 top_left_corner, float height, float width, glm::vec3 color, bool fill)
	: Rectangle(name, top_left_corner, height, width, color, fill)
{
}


Brick::~Brick()
{
}

void Brick::Update(float delta_time_seconds) {
	model_matrix_ = glm::mat3(1);
}
