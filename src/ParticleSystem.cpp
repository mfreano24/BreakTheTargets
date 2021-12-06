#include "ParticleSystem.h"
using namespace std;
using namespace glm;

void ParticleSystem::draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog)
{

	glEnableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(prog->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(prog->getAttribute("aTexInd"));
	glBindBuffer(GL_ARRAY_BUFFER, texIndBufID);
	glBufferData(GL_ARRAY_BUFFER, texIndBuf.size() * sizeof(int), &texIndBuf[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(prog->getAttribute("aTexInd"), 1, GL_FLOAT, GL_FALSE, 0, 0);


	glDrawArrays(GL_POINTS, 0, 3 * n);

	glDisableVertexAttribArray(prog->getAttribute("aPos"));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::step(float dt, shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, shared_ptr<Program> prog, 
	vec3& billboard_dir)
{
	if (!isActive) {
		return; //this shouldn't really ever be encountered but safety first
	}

	timer += dt;
	if (timer > 1.0f) {
		//do not render, return the function, it is done
		cerr << "Particles stopped!" << endl;
		particles.clear();
		isActive = false;
		return;
	}

	//step the particles, update the position buffer
	for (auto p : particles) {
		p->step(dt);
		posBuf[3 * p->index + 0] = p->x.x;
		posBuf[3 * p->index + 1] = p->x.y;
		posBuf[3 * p->index + 2] = p->x.z;
	}

	//render the particles
	MV->pushMatrix();
	MV->translate(center);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	prog->bind();
	//bind all 4 textures
	textures[0]->bind(prog->getUniform("texA"));
	textures[1]->bind(prog->getUniform("texB"));
	textures[2]->bind(prog->getUniform("texC"));
	textures[3]->bind(prog->getUniform("texD"));

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));


	cerr << "P[1][1] = " << (P->topMatrix())[1][1] << endl;
	int width, height;
	glfwGetWindowSize(Manager::Instance().window, &width, &height);
	glUniform2f(prog->getUniform("screenSize"), (float)width, (float)height);
	glUniform3f(prog->getUniform("startingPos"), (float)center.x, (float)center.y, (float)center.z);

	draw(P, MV, prog);

	textures[0]->unbind();
	textures[1]->unbind();
	textures[2]->unbind();
	textures[3]->unbind();

	prog->unbind();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	
	MV->popMatrix();
}

void ParticleSystem::InitializeTextureArray()
{
	//there's no way to do this iteratively because im not using linux
	textures.push_back(make_shared<Texture>());
	textures[0]->setFilename(Manager::Instance().RESOURCE_DIR + "explosion_particles/tex1.png");
	textures[0]->init();
	//units should start at 1 because the skybox is at unit 0.
	textures[0]->setUnit(1);

	textures.push_back(make_shared<Texture>());
	textures[1]->setFilename(Manager::Instance().RESOURCE_DIR + "explosion_particles/tex2.png");
	textures[1]->init();
	//units should start at 1 because the skybox is at unit 0.
	textures[1]->setUnit(2);

	textures.push_back(make_shared<Texture>());
	textures[2]->setFilename(Manager::Instance().RESOURCE_DIR + "explosion_particles/tex3.png");
	textures[2]->init();
	//units should start at 1 because the skybox is at unit 0.
	textures[2]->setUnit(3);

	textures.push_back(make_shared<Texture>());
	textures[3]->setFilename(Manager::Instance().RESOURCE_DIR + "explosion_particles/tex4.png");
	textures[3]->init();
	//units should start at 1 because the skybox is at unit 0.
	textures[3]->setUnit(4);
}

void ParticleSystem::PlayAt(vec3& _cen)
{
	center = _cen;
	/*
	if (isActive) {
		return; //TODO: clear the old explosion and play a new one here
	}
	*/


	isActive = true;
	timer = 0.0f;
	particles.clear();

	for (int i = 0; i < n; i++) {
		float rand_x = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		float rand_z = 2.0f * ((float)rand() / (float)RAND_MAX) - 1.0f;
		float rand_y = 2.0f * ((float)rand() / (float)RAND_MAX) + 1.0f;
		particles.push_back(make_shared<Particle>(vec3(0.0f), 2.5f * normalize(vec3(rand_x, rand_y, rand_z)), 0, Manager::Instance().RESOURCE_DIR, i));
	}



}

