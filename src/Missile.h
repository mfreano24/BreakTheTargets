#pragma once
#include "ParticleSystem.h"

class Missile {
public:
	bool active = false;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 left;

	std::shared_ptr<Shape> rocket;

	Missile();

	Missile(glm::vec3& _p, glm::vec3& _f, glm::vec3& _l, std::string resource_dir);

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t, glm::mat4& heliM);
};