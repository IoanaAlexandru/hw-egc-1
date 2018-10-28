#pragma once
#include "Brick Breaker/Generic/Circle.h"
#include "Wall.h"
class Ball :
	public Circle
{
public:
	float kDefaultSpeed = 10;

	// Create circle (ball) with given center, radius and color
	Ball(std::string name, glm::vec3 center, float radius, glm::vec3 color);
	~Ball();
	void Update(float delta_time_seconds);
	void StartMoving();
	void Move(float new_position);
	void OnWallCollision(Position wall_position);
	void OnPlatformHit(glm::vec3 platform_top_left_corner, float platform_size);

protected:
	float movement_x_, movement_y_;
	float movement_speed_;
	bool is_moving_;
};
