#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "Manager.h"

using namespace std;

GLFWwindow *window; // Main application window
string RESOURCE_DIR = ""; // Where the resources are loaded from

int keyPresses[256] = {0}; // only for English keyboards!

shared_ptr<Program> prog;
shared_ptr<Camera> camera;
shared_ptr<Shape> shape;

static void error_callback(int error, const char *description)
{
	cerr << description << endl;
}


int main(int argc, char **argv)
{
	if(argc < 3) {
		cout << "USAGE: ./Balloons <Resource Directory> <# of Control Point Rows>" << endl;
		return 0;
	}
	RESOURCE_DIR = argv[1] + string("/");
	int nBalloons = atoi(argv[2]);
	
	// Set error callback.
	glfwSetErrorCallback(error_callback);
	// Initialize the library.
	if(!glfwInit()) {
		return -1;
	}
	// Create a windowed mode window and its OpenGL context.
	window = glfwCreateWindow(1200, 750, "--", NULL, NULL);
	if(!window) {
		glfwTerminate();
		return -1;
	}
	// Make the window's context current.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		return -1;
	}
	glGetError(); // A bug in glewInit() causes an error that we can safely ignore.
	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	
	Manager::Instance().RESOURCE_DIR = RESOURCE_DIR; //set the resource directory.
	Manager::Instance().controlRows = nBalloons;
	Manager::Instance().GameLoop(window); //call the loop.
	
	// Quit program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
