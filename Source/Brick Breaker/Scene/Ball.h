#pragma once
#include "Brick Breaker/Generic/Circle.h"
class Ball :
	public Circle
{
public:
	float kDefaultSpeed = 5;

	// Create circle (ball) with given center, radius and color
	Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Ball();
	void Update(float delta_time_seconds);
	void StartMoving();
	void Move(float new_position);

protected:
	float movement_x_, movement_y_;
	float movement_speed_;
	bool is_moving_;
};
