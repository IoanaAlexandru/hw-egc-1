#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Brick.h"
#include "Wall.h"

class Scene : public SimpleScene
{
	public:
		Scene();
		~Scene();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		static const float
			BRICK_PANEL_WIDTH_RATIO,  // percentage of window width reserved for brick panel
			BRICK_PANEL_HEIGHT_RATIO,  // percentage of window height reserved for brick panel
			WALL_THICKNESS_RATIO,     // percentage of the smallest window dimension reserved for the wall
			BRICK_DISTANCE_RATIO;      // percentage of brick panel reserved for the space between bricks
		static const int
			BRICKS_PER_ROW,
			BRICK_ROWS;
		std::vector<Brick*> bricks;
		std::vector<Wall*> walls;
};
