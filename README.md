# Brick Breaker
This is a simple **Brick Breaker (Breakout!)** game developed in **C++** using **OpenGL** libraries, with a 3-life system and various powerups.

<div style="text-align:center"><img src ="https://image.ibb.co/i9G37f/brickbreaker2.png" /></div>

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

For the purpose of re-using certain shapes for different scene elements (as well as allowing us to easily change the shape of an element), we have several custom `AnimatedMesh`es with special shapes:
* `animatedmesh::Circle`
* `animatedmesh::Rectangle`
* `animatedmesh::Star`
* `animatedmesh::Heart`

Functions for generating matrices for various animation-related operations (`Translate`, `Scale`, `Rotate`) are found in `transform2d.h`.

## Controls
The platform (and the ball, if stuck to the platform) moves horizontally with the movement of the mouse (`OnMouseMove()`), without moving past the lateral walls. The ball is launched from the platform if any of the mouse buttons are pressed (`OnMouseBtnPress()`), and if multiple balls are on the platform at a certain time, only one of them will be launched per click. The game can be paused/unpaused by pressing the <kbd>P</kbd> key (`OnKeyPress()`).

## Gameplay
`TODO`
### Power-ups
`TODO`

## Movement and collisions
### Ball movement
Unless stuck to the platform, the balls move continuously through the scene. Each ball's movement is determined by an associated direction vector represented as two coordinates (`movement_x_`, `movement_y_`) and a vector length (`movement_speed_`)
.
<div style="text-align:center"><img src ="https://image.ibb.co/gbCzF0/ball-movement.jpg" /></div>

#### Wall collisions
When the ball hits a wall, it is reflected normally (at an equal angle). Since the speed doesn't change, this is done by simply inverting the horizontal component if the wall is vertical (`movement_x_ = -movement_x_`) or inverting the vertical component if the wall is horizontal (`movement_y_ = -movement_y`).

<div style="text-align:center"><img src ="https://i.stack.imgur.com/QGuay.png" /></div>

#### Platform collisions
When the ball hits a platform, the angle that the new direction of the ball makes with the platform has a linear increase from left to right based on the collision point: the leftmost corner of the platform would have an angle of 0°, the middle of the platform an angle of 90°, the rightmost corner of the platform an angle of 180° and everything in between.

<div style="text-align:center"><img src ="https://ocw.cs.pub.ro/courses/_media/egc/teme/fig2.png?w=750&tok=1f0928" /></div>

The new movement vector of the ball is calculated by considering the platform the cosine axis of the unit circle (with `cosine = 0` in the middle of the platform, `cosine = 1` at the rightmost corner and `cosine = -1` at the leftmost corner. The reflection angle will then be `acos(cosine)`, and the new movement coordinates will be obtained as:
~~~
movement_x_ = movement_speed_ * cos(reflect_angle);
movement_y_ = movement_speed_ * sin(reflect_angle);
~~~

#### Brick collisions
Depending on the place where the ball collides with the brick, it will reflect in the same way as if hitting a wall.
`TODO`

### Power-up movement
`TODO`
#### Platform collision
`TODO`

***
*<p align="center">(c) Ioana Alexandru | 334CB</p>*