# Brick Breaker
This is a simple **Brick Breaker (Breakout!)** game developed in **C++** using **OpenGL** libraries, with a 3-life system and various powerups.

![Brick Breaker screenshot](https://image.ibb.co/i9G37f/brickbreaker2.png)

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

***
<p align="center">*(c) Ioana Alexandru | 334CB*</p>