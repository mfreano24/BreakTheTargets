#include "UIManager.h"

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
		string currFile = RESOURCE_DIR + to_string(i) + ".png";
		digits[i]->setFilename(currFile);
		cerr << "loading " << currFile << " into UI thread @ unit " << (5 + i) << endl;
		digits[i]->init();
		digits[i]->setUnit(5 + i); //start at 5?
	}

	//initialize texture program for drawing quads with textures
	textureProg = make_shared<Program>();
	textureProg->setShaderNames(RESOURCE_DIR + "UI_vert.glsl", RESOURCE_DIR + "UI_frag.glsl");

	textureProg->setVerbose(true);
	textureProg->init();
	textureProg->addUniform("MV");
	textureProg->addUniform("tex");
	textureProg->addAttribute("aPos");
	textureProg->setVerbose(false);

}

void UIManager::renderUI()
{
	double t = glfwGetTime();

	#pragma region RenderRemainingTargets
	//get the textures necessary
	vector< shared_ptr<Texture> > currdigits;
	int x = 687;
	while (x > 0) {
		cerr << x % 10 << " ";
		currdigits.push_back(digits[x % 10]);
		x /= 10;
	}
	cerr << endl << endl;

	//NOTE FOR FUTURE SELF: when iterating through these, iterate BACKWARDS PLEASE!

	//get top right screen pos MINUS SOME MARGIN
	//make a step that moves the position to the left by some number

	for (int i = currdigits.size() - 1; i >= 0; i--) {

	}
	#pragma endregion

}
