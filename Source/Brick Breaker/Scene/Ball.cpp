#include "Ball.h"
#include "Brick Breaker/Generic/Transform2D.h"

Ball::Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color)
	: Circle(name, center, radius, color)
{
	movement_speed_ = kDefaultSpeed;
}

Ball::~Ball()
{
}

void Ball::Update(float delta_time_seconds) {
	if (is_moving_) {
		model_matrix_ *= Transform2D::Translate(movement_x_, movement_y_);
	}
}

void Ball::StartMoving() {
	if (!is_moving_) {
		is_moving_ = true;
		movement_y_ = movement_speed_;
	}
}

void Ball::Move(float new_position) {
	if (!is_moving_) {
		model_matrix_ = Transform2D::Translate(new_position, 0);
	}
}
