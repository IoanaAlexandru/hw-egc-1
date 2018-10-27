#pragma once
#include "Core/GPU/Mesh.h"
class AnimatedMesh :
	public Mesh
{
public:
	AnimatedMesh(std::string name) : Mesh(name) {};
	~AnimatedMesh() {};

	virtual void Update(float delta_time_seconds) = 0;

	glm::mat3 GetModelMatrix() { return model_matrix_; }

protected:
	glm::mat3 model_matrix_ = glm::mat3(1);
};

