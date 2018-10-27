#include "Platform.h"

#include <Core/Engine.h>

Platform::Platform(std::string name, glm::vec3 topLeftCorner, float height, float width, glm::vec3 color, bool fill)
	: Rectangle(name, topLeftCorner, height, width, color, fill)
{
}

Platform::~Platform()
{
}
