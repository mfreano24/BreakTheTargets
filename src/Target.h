#pragma once
#include "ParticleSystem.h"


class Target {
public:
	glm::vec3 pos;

	std::shared_ptr<Shape> sphere;

	bool active;

	Target();

	Target(glm::vec3& _p, std::string resource_dir);

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t);
	void CheckCollision(glm::vec3& _missilePos, std::shared_ptr<ParticleSystem> ps);
};

