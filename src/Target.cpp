#include "Target.h"
#include "UIManager.h"

using namespace std;
using namespace glm;

Target::Target()
{
	pos = vec3(0.0f);
}

Target::Target(glm::vec3& _p, std::string resource_dir)
{
	pos = _p;

	basePosition = _p;

	period = 0.25f + 5.0f * ((float)rand() / ((float)RAND_MAX - 1.0f));

	amplitude = 1.0f * 20.0f * ((float)rand() / ((float)RAND_MAX - 1.0f));

	sphere = make_shared<Shape>();
	sphere->loadMesh(resource_dir + "sphere.obj");
	sphere->init();

	for (int i = 0; i < 3; i++) {
		int rng = rand() % 10;
		if (rng < 5) {
			movedir[i] = true;
		}
		else {
			movedir[i] = false;
		}

		cycleOffset[i] = 0.1f * (float)rng;
	}

	randomColor = vec3((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX);

	//cerr << "random color: " << randomColor.x << ", " << randomColor.y << ", " << randomColor.z << endl;

	active = true;
}

vec3 Target::CalculatePosition(double t)
{
	//x
	float nX = basePosition.x, nY = basePosition.y, nZ = basePosition.z;
	if (movedir[0]) {
		nX += amplitude * sin(period * (t + cycleOffset[0]));
	}
	//y

	if (movedir[1]) {
		nY += amplitude * sin(period * (t + cycleOffset[1]));
	}

	if (movedir[2]) {
		nZ += amplitude * sin(period * (t + cycleOffset[2]));
	}

	//z

	return vec3(nX, nY, nZ);
}

void Target::draw(shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> MV, shared_ptr<Program> prog, double t)
{
	prog->bind();
	MV->pushMatrix();
	pos = CalculatePosition(t);
	MV->translate(pos);
	MV->scale(10.0f);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), randomColor.x, randomColor.y, randomColor.z);
	sphere->draw(prog);
	MV->popMatrix();
}

bool Target::CheckCollision(glm::vec3& missilePos, std::shared_ptr<ParticleSystem> ps) {
	float dist_sq = pow(missilePos.x - pos.x, 2) + pow(missilePos.y - pos.y, 2) + pow(missilePos.z - pos.z, 2);
	//cerr << "missile pos : " << missilePos.x << ", " << missilePos.y << ", " << missilePos.z << " || " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	if (dist_sq <= 110.0f) {
		//collide!
		UIManager::Instance().DecrementTargets();
		active = false;

		ps->kd = randomColor;
		ps->PlayAt(pos);
		return true;
	}
	return false;
}


