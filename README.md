# Brick Breaker
This is a simple **Brick Breaker (Breakout!)** game developed in **C++** using **OpenGL** libraries, with a 3-life system and various powerups.

<div style="text-align:center"><img src ="https://image.ibb.co/i9G37f/brickbreaker2.png" /></div>

## Table of contents
* [Project structure](#project-structure)
* [Controls](#controls)
* [Gameplay](#gameplay)
	* [Power-ups](#power-ups)
* [Movement and collisions](#movement-and-collisions)
	* [Ball movement](#ball-movement)
		* [Wall collisions](#wall-collisions)
		* [Platform collisions](#platform-collisions)
		* [Brick collisions](#brick-collisions)
	* [Power-up movement](#power-up-movement)
		* [Platform collisions](#platform-collisions-1)

## Project structure
The game itself is an instance of a `brickbreaker::Scene`, a special type of `SimpleScene`. This scene contains:
* UI elements (they don't affect the actual gameplay)
    * Lives (`brickbreaker::Life`)
    * Pause button (`brickbreaker::Pausebutton`)
* Game elements
    * Bricks (`brickbreaker::Brick`)
    * Balls (`brickbreaker::Ball`)
    * Platform (`brickbreaker::Platform`)
    * Power-ups (`brickbreaker::Powerup`)
    * Walls (`brickbreaker::Wall`)

All of these elements are either regular `Mesh`es or `AnimatedMesh`es. An `AnimatedMesh` is a special type of `Mesh` which is animated, meaning it holds a model matrix which dictates the position/size etc. of the mesh at one specific point in time. When rendering, a regular `Mesh` will use a model matrix that is simply the identity matrix (`glm::mat3(1)`), while an `AnimatedMesh` uses its own model matrix.

In order to keep different elements of the game separate, the project implements a variation of the **Model-View-Controller** design pattern, where the elements listed above are part of the Model, the View is the actual rendering of these elements (done through the `RenderMesh2D()` method call), and the controller is the `brickbreaker::Scene`. Hence, none of the elements listed above know about each other, and the `brickbreaker::Scene` is in charge of managing the interactions between them.

For the purpose of re-using certain shapes for different scene elements (as well as allowing us to easily change the shape of an element), we have several custom `AnimatedMesh`es with special shapes:
* `animatedmesh::Circle`
* `animatedmesh::Rectangle`
* `animatedmesh::Star`
* `animatedmesh::Heart`

Functions for generating matrices for various animation-related operations (`Translate`, `Scale`, `Rotate`) are found in `transform2d.h`.

## Controls
The platform (and the ball, if stuck to the platform) moves horizontally with the movement of the mouse (`OnMouseMove()`), without moving past the lateral walls. The ball is launched from the platform if any of the mouse buttons are pressed (`OnMouseBtnPress()`), and if multiple balls are on the platform at a certain time, only one of them will be launched per click. The game can be paused/unpaused by pressing the <kbd>P</kbd> key (`OnKeyPress()`).

## Gameplay
The player starts with one ball and their purpose is to destroy all the bricks in the scene by hitting them with the ball. If a ball falls and no other balls are left in the scene, the player loses a life. If all three lives are lost, the game starts over.
### Power-ups
The game can be made easier or harder by catching power-ups/power-downs, which have a chance to spawn whenever a brick is destroyed. A power-up has to touch the platform in order to activate, and most power-ups will automatically de-activate after 10 seconds. The power-up's effects are incremental.

The following power-ups are available:
* **Platform stretch** - makes the platform larger for easier control of the ball(s)
* **Platform shrink** - makes the platform smaller
* **Bottom wall** - a wall appears on the bottom of the scene, preventing balls from falling
* **Extra ball** -  a new ball can be launched in the game together with the existing ones

In order to determine whether or not to spawn a power-up when a brick is destroyed, and which specific effect that power-up should have, `std::default_random_engine` and `std::bernoulli_distibution` are used.

## Movement and collisions
### Ball movement
Unless stuck to the platform, the balls move continuously through the scene. Each ball's movement is determined by an associated direction vector represented as two coordinates (`movement_x_`, `movement_y_`) and a vector length (`movement_speed_`)
.
<div style="text-align:center"><img src ="https://image.ibb.co/gbCzF0/ball-movement.jpg" /></div>

Hence, with every call to `Update()`, a new matrix for translation with `movement_x_`, `movement_y_` is multiplied with the ball's model matrix, and the ball's `center` is updated.

#### Wall collisions
* `bool Scene::CheckCollision(
      Ball *ball,
      std::pair<const animatedmesh::Position, brickbreaker::Wall *> wall)`
* `void Ball::OnHit(animatedmesh::Position obstacle_position)`

When the ball hits a wall, it is reflected normally (at an equal angle). Since the speed doesn't change, this is done by simply inverting the horizontal component if the wall is vertical (`movement_x_ = -movement_x_`) or inverting the vertical component if the wall is horizontal (`movement_y_ = -movement_y`).

<div style="text-align:center"><img src ="https://i.stack.imgur.com/QGuay.png" /></div>

#### Platform collisions
* `bool Scene::CheckCollision(Ball *ball, Platform *platform)`
* `void Ball::OnPlatformHit(glm::vec3 platform_center, float platform_size)`

When the ball hits a platform, the angle that the new direction of the ball makes with the platform has a linear increase from left to right based on the collision point: the leftmost corner of the platform would have an angle of 0°, the middle of the platform an angle of 90°, the rightmost corner of the platform an angle of 180° and everything in between.

<div style="text-align:center"><img src ="https://ocw.cs.pub.ro/courses/_media/egc/teme/fig2.png" /></div>

The new movement vector of the ball is calculated by considering the platform the cosine axis of the unit circle (with `cosine = 0` in the middle of the platform, `cosine = 1` at the rightmost corner and `cosine = -1` at the leftmost corner. The reflection angle will then be `acos(cosine)`, and the new movement coordinates will be obtained as:
~~~
movement_x_ = movement_speed_ * cos(reflect_angle);
movement_y_ = movement_speed_ * sin(reflect_angle);
~~~

#### Brick collisions
* `bool Scene::Collide(Ball *ball, Brick *brick,
                    animatedmesh::Position brick_position)`
* `bool Scene::CheckCollision(Ball *ball, Brick *brick)`
* `void Ball::OnHit(animatedmesh::Position obstacle_position)`

When a ball hits a brick, the brick will disappear by gradually shrinking. Therefore, when a brick is hit, its `Update()` function will first translate the brick to the origin, scale it and then translate it back to its place. This is done by gradually multiplying the model matrix with the translation and scale matrices in reverse order.

Depending on the place where the ball collides with the brick, it will reflect in the same way as if hitting a wall. We need multiple conditions in order to determine which direction the ball is reflected in.

If we consider absolute distances (we are particularly interested in the distance between the center of the brick and the center of the ball), we can limit the number of separate conditions by only working with one quadrant instead of four to determine whether the ball is actually touching the platform (we can then use the quadrant to determine the direction of the reflection).

<div style="text-align:center"><img src ="https://image.ibb.co/iw5o7f/brickbreaker.jpg" /></div>

In the image above, the grey area is the upper-right quadrant of the brick and the red line is the limit of where the center of the ball can be so it intersects with the brick. We can eliminate the green area (where we know for sure that the ball isn't touching the brick) straight away:
~~~
if (abs_centers_distance.x > brick_width_ / 2 + ball_radius_) return false;
if (abs_centers_distance.y > brick_height_ / 2 + ball_radius_) return false;
~~~
In the two orange areas the ball is clearly touching the brick, so we just need to determine the direction in which the ball should be reflected by using the sign of the difference between the ball's coordinates and the brick's coordinates.
~~~
if (abs_centers_distance.y <= brick_height_ / 2)
    if (centers_distance.x > 0)
      // reflect to the right
    else
      // reflect to the left
  if (abs_centers_distance.x <= brick_width_ / 2)
    if (centers_distance.y > 0)
      // reflect down
    else
      // reflect up
~~~
In the white area, the euclidean distance between the center of the ball and the corner of the brick has to be smaller than the radius of the ball. The direction of reflection (up/right in the quadrant displayed above) is determined by whether the center of the ball is over or under the dotted line (which can be determined by using the difference between the x and the y of the difference between the center of the ball and the corner of the brick):
~~~
if (GetEuclideanDistance(corner_distance) <= ball_radius_)
    if (corner_distance.x < corner_distance.y)
      if (centers_distance.x > 0)
        // reflect right
      else
        // reflect left
    else if (centers_distance.y > 0)
      // reflect down
    else
      // reflect up
~~~


### Power-up movement
When a power-up spawns, it will spawn in the center of the destroyed brick and gradually fall down while rotating around its axis. In order to obtain this animation, the power-up has a rotation `angle_` (that increments gradually) together with the associated `center_`. With every call to `Update()`, the model matrix will be the result of translating the power-up to the origin, rotating it and then translating it to the desired spot.
#### Platform collisions
When a power-up hits the platform, it shrinks and its effect is activated. In addition to the operations above, the power-up is moved again to the origin, scaled and then moved to the desired location.

***
*<p align="center">(c) Ioana Alexandru | 334CB</p>*