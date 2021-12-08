#pragma once
#include "ParticleSystem.h"


class Target {
public:
	glm::vec3 pos;

	std::shared_ptr<Shape> sphere;

	bool active;

	glm::vec3 randomColor;

	glm::vec3 basePosition;

	//sinusoid movement
	float amplitude;
	float period;

	float cycleOffset[3];

	//direction
	bool movedir[3];

	Target();

	Target(glm::vec3& _p, std::string resource_dir);

	glm::vec3 CalculatePosition(double t);

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t);
	void CheckCollision(glm::vec3& _missilePos, std::shared_ptr<ParticleSystem> ps);
};

