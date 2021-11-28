#include "Manager.h"
#include <thread>
using namespace std;
using namespace glm;

void Manager::init_meshcreator(){
    
    GLSL::checkVersion();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set background color
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Enable z-buffer test
	glEnable(GL_DEPTH_TEST);

	keyPresses[(unsigned)'c'] = 1;


	
	camera = make_shared<Camera>();

	InitializeShaders();

	skybox = make_shared<Shape>();
	skybox->loadMesh(RESOURCE_DIR + "cube.obj");
	skybox->init();


	
	//initialize the noise mesh.
	if(posBuf.empty()){
		UpdateNoiseMesh();
	}

	// Initialize time.
	glfwSetTime(0.0);


	
	
	// If there were any OpenGL errors, this will print something.
	// You can intersperse this line in your code to find the exact location
	// of your OpenGL error.
	//cerr << "initializing mesh creator..." << endl;
	GLSL::checkError(GET_FILE_LINE);
	//cerr << "clear!" << endl;
	
}


void Manager::render_meshcreator(){
    // Update timers
	double t = glfwGetTime();
	
	#pragma region SettingUp
	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	
	// Use the window size for camera.
	glfwGetWindowSize(window, &width, &height);
	camera->setAspect((float)width/(float)height);
	
	// Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(keyPresses[(unsigned)'c'] % 2) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}
	if(keyPresses[(unsigned)'z'] % 2) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	auto P = make_shared<MatrixStack>();
	auto MV = make_shared<MatrixStack>();
	
	// Apply camera transforms
	P->pushMatrix();
	camera->applyProjectionMatrix(P);
	MV->pushMatrix();
	camera->applyViewMatrix(MV);
	MV->pushMatrix();

    #pragma endregion

	#pragma region RenderMesh
	worldprog->bind();
	MV->pushMatrix();

	MV->translate(glm::vec3(-10.0f,-5.0f,-10.0f));
	MV->scale(5.0f);

	glUniformMatrix4fv(worldprog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(worldprog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glUniform3f(worldprog->getUniform("kd"), 0.5f, 0.5f, 1.0f);

	//cerr << "checking noiseMesh->draw... ";
	noiseMesh->draw(prog);
	//cerr << "clear!" << endl;

	MV->popMatrix();
	worldprog->unbind();
	#pragma endregion

#pragma region RenderSkybox
	skyboxprog->bind();
	MV->pushMatrix();
	MV->scale(500.0f);
	//MV->rotate(glm::pi<float>(), vec3(1.0, 0.0, 0.0));
	if (keyPresses[(unsigned)'c'] % 2) {
		glDisable(GL_CULL_FACE);
	}
	//glDepthMask(GL_FALSE);
	glUniformMatrix4fv(skyboxprog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	glUniformMatrix4fv(skyboxprog->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
	glUniform1i(skyboxprog->getUniform("skybox"), 0);
	GLSL::checkError(GET_FILE_LINE);
	skybox->draw(skyboxprog);
	//glDepthMask(GL_TRUE);
	if (keyPresses[(unsigned)'c'] % 2) {
		glEnable(GL_CULL_FACE);
	}
	skyboxprog->unbind();
	MV->popMatrix();

#pragma endregion

	MV->popMatrix();
	P->popMatrix();


	//cerr << "checking error in render_meshcreator().." << endl;
	GLSL::checkError(GET_FILE_LINE);
	//cerr << "clear!\n" << endl;
}