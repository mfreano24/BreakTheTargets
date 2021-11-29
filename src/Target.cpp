#include "Target.h"

using namespace std;
using namespace glm;

Target::Target()
{
	pos = vec3(0.0f);
}

Target::Target(glm::vec3& _p, std::string resource_dir)
{
	pos = _p;

	sphere = make_shared<Shape>();
	sphere->loadMesh(resource_dir + "sphere.obj");
	sphere->init();

	active = true;
}

void Target::draw(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, shared_ptr<Program> prog, double t)
{
	prog->bind();
	MV->pushMatrix();
	MV->translate(pos);
	MV->scale(10.0f);
	
	
	
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.5f, 0.0f);
	sphere->draw(prog);
	MV->popMatrix();
}

void Target::CheckCollision(glm::vec3& missilePos, std::shared_ptr<ParticleSystem> ps) {
	float dist_sq = pow(missilePos.x - pos.x, 2) + pow(missilePos.y - pos.y, 2) + pow(missilePos.z - pos.z, 2);
	cerr << "missile pos : " << missilePos.x << ", " << missilePos.y << ", " << missilePos.z << " || " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	if (dist_sq <= 100.0f) {
		//collide!
		active = false;
		ps->PlayAt(pos);
		Manager::Instance().DestroyMissile();
	}
}


