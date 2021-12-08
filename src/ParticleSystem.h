#pragma once
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "MatrixStack.h"
#include "Shape.h"
#include "Program.h"
#include "GLSL.h"
#include "Texture.h"
#include "Particle.h"
#include "Manager.h"

class ParticleSystem {
private:
	std::vector<float> posBuf;
	std::vector<float> texIndBuf;
	GLuint posBufID;
	GLuint texIndBufID;
	void InitializeTextureArray();
public:

	std::vector< std::shared_ptr<Particle> > particles;
	std::vector< std::shared_ptr<Texture> > textures;
	bool isActive;
	glm::vec3 center;
	float timer;
	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog);
	void step(float dt, std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, glm::vec3& billboard_dir);
	int n;
	void PlayAt(glm::vec3& _cen);

	ParticleSystem() : isActive(false), center(glm::vec3(0.0f)), timer(0.0f), n(50) {
		InitializeTextureArray();
		posBuf.resize(3 * n);
		glGenBuffers(1, &posBufID);

		texIndBuf.resize(n);
		glGenBuffers(1, &texIndBufID);

		for (int i = 0; i < n; i++) {
			texIndBuf[i] = floor(rand() % 4);
		}
	}
};