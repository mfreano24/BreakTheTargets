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

void PrintMatrix(mat4& m) {
	cerr << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << endl;
	cerr << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << endl;
	cerr << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << endl;
	cerr << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << endl;
}

void Missile::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t, mat4& heliM)
{
	prog->bind();
	MV->pushMatrix();
	MV->translate(pos);
	heliM = mat4(mat3(heliM));

	MV->multMatrix(heliM);
	//rotate towards the direction we're going?
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 0.0f, 0.0f, 0.0f);
	rocket->draw(prog);
	MV->popMatrix();
}



