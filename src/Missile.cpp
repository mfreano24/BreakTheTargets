#include "Missile.h"

using namespace std;
using namespace glm;

Missile::Missile()
{
	pos = vec3(0.0f);
	forward = vec3(1.0f, 0.0f, 0.0f);
	left = vec3(0.0f, 0.0f, -1.0f);
}

Missile::Missile(glm::vec3& _p, glm::vec3& _f, glm::vec3& _l, std::string resource_dir)
{
	pos = _p;
	forward = _f;
	left = _l;

	rocket = make_shared<Shape>();
	rocket->loadMesh(resource_dir + "rocket.obj");
	rocket->init();
}

void Missile::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t)
{
	prog->bind();
	MV->pushMatrix();
	MV->translate(pos);
	MV->rotate(pi<float>() / 2.0f, normalize(left));
	
	//rotate towards the direction we're going?
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 1.0f, 0.0f);
	rocket->draw(prog);
	MV->popMatrix();
}



