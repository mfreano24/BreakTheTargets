#include "UIManager.h"
#include <stack>
#include "GLSL.h"
using namespace std;
using namespace glm;


void UIManager::init(GLFWwindow* _win, string& rez)
{
	window = _win;
	RESOURCE_DIR = rez + "UI/";
	cerr << RESOURCE_DIR << endl;
	int ht, wd;
	glfwGetWindowSize(window, &ht, &wd);
	cerr << ht << " x " << wd << endl;
	//set up the digit textures
	digits.resize(10); //0 through 9 inclusive
	for (int i = 0; i <= 9; i++) {
		
		digits[i] = make_shared<Texture>();
		string currFile = RESOURCE_DIR + to_string(i) + ".jpg";
		digits[i]->setFilename(currFile);
		cerr << "loading " << currFile << " into UI thread @ unit " << (5 + i) << endl;
		digits[i]->init();
		digits[i]->setUnit(1); //start at 5?
	}

	//initialize texture program for drawing quads with textures
	textureProg = make_shared<Program>();
	textureProg->setShaderNames(RESOURCE_DIR + "UI_vert.glsl", RESOURCE_DIR + "UI_frag.glsl");

	textureProg->setVerbose(true);
	textureProg->init();
	textureProg->addUniform("MV");
	textureProg->addUniform("tex");
	textureProg->addUniform("screenSize");
	textureProg->addAttribute("aPos");
	textureProg->setVerbose(false);

	quad = make_shared<Shape>();
	quad->loadMesh(rez + "quad.obj");
	quad->init();


	glGenBuffers(1, &posBufID);
	posBuf = vector<float>{ 0.0f, 0.0f, 0.0f }; //centrally locate, focus on using MV to calculate screen pos

}

void UIManager::renderUI()
{
	double t = glfwGetTime();

	#pragma region RenderRemainingTargets
	//get the textures necessary
	stack<int> digs;
	int x = 22;
	while (x > 0) {
		cerr << x % 10 << " ";
		digs.push(x % 10);
		x /= 10;
	}
	cerr << endl << endl;

	//NOTE FOR FUTURE SELF: when iterating through these, iterate BACKWARDS PLEASE!

	//get top right screen pos MINUS SOME MARGIN
	//make a step that moves the position to the left by some number
	int h, w;
	glfwGetWindowSize(window, &w, &h);
	shared_ptr<MatrixStack> MV = make_shared<MatrixStack>();
	int margin = 200;
	int xpos = w - margin, ypos = h - margin;
	float xcorner = (float)xpos / w, ycorner = (float)ypos / h;

	float totDigitOffset = 0.0f, offsetStep = 0.1f;
	while (!digs.empty()) {
		int cur = digs.top();
		digs.pop();
		textureProg->bind();
		MV->pushMatrix();
		MV->translate(xcorner + totDigitOffset, ycorner, 0.0f);
		totDigitOffset -= offsetStep;
		MV->scale(0.1f, 0.2f, 1.0f);
		MV->scale(0.5f);
		MV->rotate(-pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
		
		

		digits[cur]->bind(textureProg->getUniform("tex"));
		cerr << digits[cur]->getFileName() << endl;
		glUniformMatrix4fv(textureProg->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
		glUniform2f(textureProg->getUniform("screenSize"), (float)w, (float)h);

		//posBuf
		glEnableVertexAttribArray(textureProg->getAttribute("aPos"));
		glBindBuffer(GL_ARRAY_BUFFER, posBufID);
		glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(textureProg->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);


		glDrawArrays(GL_POINTS, 0, 3);

		glDisableVertexAttribArray(textureProg->getAttribute("aPos"));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		digits[cur]->unbind();
		MV->popMatrix();
		textureProg->unbind();

		GLSL::checkError(GET_FILE_LINE);
	}
	#pragma endregion

}
