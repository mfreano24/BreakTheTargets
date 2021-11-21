#include "Particle.h"
using namespace std;
using namespace glm;

void Particle::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Texture> thisTex, std::shared_ptr<Program> prog,
	vec3& billboard_dir)
{
	prog->bind();
	//pass projection to gpu
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));

	
	MV->pushMatrix();
	//billboarding
	//TODO: NOT ACTUALLY BILLBOARDING
	
	MV->translate(x);
	MV->scale(0.65f);
	//MV->multMatrix(rot);
	
	//move to our position
	

	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));

	thisTex->bind(prog->getUniform("texture"));

	quad->draw(prog);

	MV->popMatrix();
	thisTex->unbind();
	prog->unbind();
}

void Particle::step(float dt)
{
	//update positions
	x += dt * v;

	//update velocities
	//gravity seems to be backwards. yes this is entirely intentional, but i just want my project to look good, man.
	v -= dt * vec3(0.0f, 60.0f, 0.0f);
}

void Particle::InitializeQuad(string resource_dir)
{
	quad = make_shared<Shape>();
	quad->loadMesh(resource_dir + "quad.obj");
	quad->init();
}
