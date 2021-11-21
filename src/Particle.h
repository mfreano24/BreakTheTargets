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
#include "Camera.h"
#include "GLSL.h"
#include "Texture.h"

class Particle {
private:
	void InitializeQuad(std::string resource_dir);
public:
	glm::vec3 x;
	glm::vec3 v; //should signify a direction too.
	int index;

	int texIndex;

	std::shared_ptr<Shape> quad;

	void draw(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Texture> thisTex, std::shared_ptr<Program> prog,
		glm::vec3& billboard_dir);

	void step(float dt);

	Particle() : x(glm::vec3(0.0f)), v(normalize(glm::vec3(0.0f, 1.0f, 0.0f))), texIndex(0), index(0) {
		InitializeQuad(""); //this would fail??? hm.
	}

	Particle(glm::vec3& _x, glm::vec3& _v0, int ind, std::string res, int _i) : x(_x), v(15.0f * _v0), texIndex(ind), index(_i) {
		InitializeQuad(res);
	}



	
};