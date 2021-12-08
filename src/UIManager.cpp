#include "UIManager.h"
#include <stack>
#include <queue>
#include "GLSL.h"
#include "Manager.h"
using namespace std;
using namespace glm;


void UIManager::init(GLFWwindow* _win, string& rez)
{
	window = _win;
	RESOURCE_DIR = rez + "UI/";
	cerr << RESOURCE_DIR << endl;
	int ht, wd;
	glfwGetWindowSize(window, &ht, &wd);
	//cerr << ht << " x " << wd << endl;
	//set up the digit textures
	digits.resize(10); //0 through 9 inclusive
	for (int i = 0; i <= 9; i++) {
		
		digits[i] = make_shared<Texture>();
		string currFile = RESOURCE_DIR + to_string(i) + ".png";
		digits[i]->setFilename(currFile);
		digits[i]->init();
		digits[i]->setUnit(1); //start at 5?
	}

	//initialize texture program for drawing quads with textures
	textureProg = make_shared<Program>();
	textureProg->setShaderNames(RESOURCE_DIR + "UI_vert.glsl", RESOURCE_DIR + "UI_frag.glsl");

	textureProg->setVerbose(true);
	textureProg->init();
	textureProg->addUniform("P");
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

	remainingTargets = Manager::Instance().targets.size();
	
	assert(remainingTargets > 0);

	int numPositions = to_string(remainingTargets).length();



	int h, w;
	glfwGetWindowSize(window, &w, &h);
	int margin = 125;
	int xpos = w - margin, ypos = h - margin + 25;
	float xcorner = (float)xpos / w, ycorner = (float)ypos / h;
	float totDigitOffset = 0.0f, offsetStep = 0.045f;

	for (int i = 0; i < numPositions; i++) {
		//cerr << "digit position: " << xcorner + totDigitOffset << ", " << ycorner << ", " << 0.0f << endl;
		digitPositions.push_back(vec3(xcorner + totDigitOffset, ycorner, 0.0f));
		totDigitOffset -= offsetStep;
	}

}

void UIManager::renderUI()
{
	double t = glfwGetTime();

	#pragma region RenderRemainingTargets
	//get the textures necessary
	queue<int> digs;
	int x = remainingTargets;
	while (x > 0) {
		//cerr << x % 10 << " ";
		digs.push(x % 10);
		x /= 10;
	}

	//NOTE FOR FUTURE SELF: when iterating through these, iterate BACKWARDS PLEASE!

	//get top right screen pos MINUS SOME MARGIN
	//make a step that moves the position to the left by some number
	int h, w;
	glfwGetWindowSize(window, &w, &h);
	shared_ptr<MatrixStack> MV = make_shared<MatrixStack>();
	MV->pushMatrix();
	int margin = 200;
	int xpos = w - margin, ypos = h - margin;
	float xcorner = (float)xpos / w, ycorner = (float)ypos / h;

	

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_POINT_SPRITE);

	mat4 P = glm::ortho(0, 1, 0, 1, -1, 1);
	int p_index = 0;
	//cerr << "DIGIT POSITION ROOT: " << digitPositions[0].x << ", " << digitPositions[0].y << ", " << digitPositions[0].z << endl;
	while (!digs.empty()) {
		int cur = digs.front();
		digs.pop();
		textureProg->bind();
		MV->pushMatrix();
		MV->translate(digitPositions[p_index++]);
		MV->scale(0.1f, 0.1f, 1.0f);
		MV->rotate(-pi<float>()/2.0f, vec3(0.0f, 1.0f, 0.0f));
		
		
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		digits[cur]->bind(textureProg->getUniform("tex"));
		//cerr << digits[cur]->getFileName() << endl;
		glUniformMatrix4fv(textureProg->getUniform("P"), 1, GL_FALSE, value_ptr(P));
		glUniformMatrix4fv(textureProg->getUniform("MV"), 1, GL_FALSE, value_ptr(MV->topMatrix()));
		glUniform2f(textureProg->getUniform("screenSize"), (float)w, (float)h);

		//posBuf
		glEnableVertexAttribArray(textureProg->getAttribute("aPos"));
		glBindBuffer(GL_ARRAY_BUFFER, posBufID);
		glBufferData(GL_ARRAY_BUFFER, posBuf.size() * sizeof(float), &posBuf[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(textureProg->getAttribute("aPos"), 3, GL_FLOAT, GL_FALSE, 0, 0);


		glDrawArrays(GL_POINTS, 0, 1);

		glDisableVertexAttribArray(textureProg->getAttribute("aPos"));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		digits[cur]->unbind();
		MV->popMatrix();
		textureProg->unbind();

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);


		GLSL::checkError(GET_FILE_LINE);
	}


	MV->popMatrix();

	glDisable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	#pragma endregion

}
