#include "Manager.h"
#include <assert.h>

using namespace std;
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma region Callbacks

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if(key == GLFW_KEY_ENTER && action == GLFW_PRESS){
		Manager::Instance().inMeshCreatorMode = !Manager::Instance().inMeshCreatorMode;
		Manager::Instance().initTrigger = true;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        Manager::Instance().DebugRKey(); //call this
	}

    #pragma region Controls
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        Manager::Instance().deltaYRot = glm::pi<float>() * (float)1e-2;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        Manager::Instance().deltaYRot = 0.0f;
    }

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		Manager::Instance().deltaYRot = -glm::pi<float>() * (float)1e-2;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		Manager::Instance().deltaYRot = 0.0f;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		Manager::Instance().deltaXRot = glm::pi<float>() * (float)1e-2;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		Manager::Instance().deltaXRot = 0.0f;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		Manager::Instance().deltaXRot = -glm::pi<float>() * (float)1e-2;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		Manager::Instance().deltaXRot = 0.0f;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		Manager::Instance().deltaZRot = glm::pi<float>() * (float)1e-2;
	}
	if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
		Manager::Instance().deltaZRot = 0.0f;
	}

    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		Manager::Instance().deltaZRot = -glm::pi<float>() * (float)1e-2;
	}
	if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
		Manager::Instance().deltaZRot = 0.0f;
	}

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        
    }


    #pragma endregion


}

static void char_callback(GLFWwindow *window, unsigned int key)
{
	Manager::Instance().keyPresses[key]++;
}

static void cursor_position_callback(GLFWwindow* window, double xmouse, double ymouse)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS) {
		Manager::Instance().camera->mouseMoved(xmouse, ymouse);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// Get the current mouse position.
	double xmouse, ymouse;
	glfwGetCursorPos(window, &xmouse, &ymouse);
	// Get current window size.
	int width, height;
	glfwGetWindowSize(window, &width, &height);
    if(action == GLFW_PRESS) {
		bool shift = mods & GLFW_MOD_SHIFT;
		bool ctrl  = mods & GLFW_MOD_CONTROL;
		bool alt   = mods & GLFW_MOD_ALT;
		Manager::Instance().camera->mouseClicked(xmouse, ymouse, shift, ctrl, alt);
	}

    
}
#pragma endregion

void Manager::InitializeShaders()
{
	worldprog = make_shared<Program>();
	worldprog->setShaderNames(RESOURCE_DIR + "world_vert.glsl", RESOURCE_DIR + "world_frag.glsl");

	worldprog->setVerbose(true);
	worldprog->init();
	worldprog->addUniform("P");
	worldprog->addUniform("MV");
    worldprog->addUniform("V");
	worldprog->addUniform("kd");
	worldprog->addAttribute("aPos");
	worldprog->addAttribute("aNor");
	worldprog->setVerbose(false);

	prog = make_shared<Program>();
	prog->setShaderNames(RESOURCE_DIR + "phong_vert.glsl", RESOURCE_DIR + "phong_frag.glsl");

	prog->setVerbose(true);
	prog->init();
	prog->addUniform("P");
	prog->addUniform("MV");
	prog->addUniform("kd");
	prog->addAttribute("aPos");
	prog->addAttribute("aNor");
	prog->setVerbose(false);

	//skybox

	skyboxprog = make_shared<Program>();
	skyboxprog->setShaderNames(RESOURCE_DIR + "skybox_vert.glsl", RESOURCE_DIR + "skybox_frag.glsl");

	skyboxprog->setVerbose(true);
	skyboxprog->init();
	skyboxprog->addUniform("P");
	skyboxprog->addUniform("MV");
	skyboxprog->addUniform("skybox");
	skyboxprog->addAttribute("aPos");
	skyboxprog->addAttribute("aNor");

	skyboxID = LoadCubeMap();

	skyboxprog->setVerbose(false);


    //explosion quad

    quadPS = make_shared<Program>();
    quadPS->setShaderNames(RESOURCE_DIR + "billboard_vert.glsl", RESOURCE_DIR + "billboard_frag.glsl");

    quadPS->setVerbose(true);
    quadPS->init();
    quadPS->addUniform("P");
    quadPS->addUniform("MV");
    quadPS->addUniform("texA");
    quadPS->addUniform("texB");
    quadPS->addUniform("texC");
    quadPS->addUniform("texD");
    quadPS->addUniform("screenSize");
    quadPS->addUniform("startingPos");
    quadPS->addAttribute("aPos");
    quadPS->addAttribute("aTexInd");

    quadPS->setVerbose(false);
}

void Manager::GameLoop(GLFWwindow* _window){
    window = _window;

    //make B spline basis
    //can change to catmull rom later if needs be
    B[0] = vec4(1.0f, 4.0f, 1.0f, 0.0f);
    B[1] = vec4(-3.0f, 0.0f, 3.0f, 0.0f);
    B[2] = vec4(3.0f, -6.0f, 3.0f, 0.0f);
    B[3] = vec4(-1.0f, 3.0f, -3.0f, 1.0f);
    B = (1.0f/6.0f) * B;

    mat4 CatmullBasis(1.0f);
    CatmullBasis[0] = vec4(0.0f, 2.0f, 0.0f, 0.0f);
    CatmullBasis[1] = vec4(-1.0f, 0.0f, 1.0f, 0.0f);
    CatmullBasis[2] = vec4(2.0f, -5.0f, 4.0f, -1.0f);
    CatmullBasis[3] = vec4(-1.0f, 3.0f, -3.0f, 1.0f);

    //B = 0.5f * CatmullBasis;

    //randomize seed
    srand(time(NULL));

    glDepthFunc(GL_LEQUAL);

    init_meshcreator();
    // Set vsync.
	glfwSwapInterval(1);
	// Set keyboard callback.
	glfwSetKeyCallback(window, key_callback);
	// Set char callback.
	glfwSetCharCallback(window, char_callback);
	// Set cursor position callback.
	glfwSetCursorPosCallback(window, cursor_position_callback);
	// Set mouse button callback.
	glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    init_meshcreator(); //this should be deprecated but id rather keep the code initializing the mesh itself here
    init_helicopter();
    while(!glfwWindowShouldClose(window)) {

        render_helicopter();
		
		// Swap front and back buffers.
		glfwSwapBuffers(window);
		// Poll for and process events.
		glfwPollEvents();
	}
}

unsigned int Manager::LoadCubeMap(){
    string folder = RESOURCE_DIR + "skybox_textures/";
    string testing = RESOURCE_DIR + "explosion_particles/help.jpg";
    cerr << "TESTING: " << testing << endl;
    unsigned int texID;
    vector<string> faces
    {
        folder + "FluffballDayRight.jpg",
        folder + "FluffballDayLeft.jpg",
        
        folder + "FluffballDayTop.jpg",
        folder + "FluffballDayBottom.jpg",
        
        folder + "FluffballDayBack.jpg",
        folder + "FluffballDayFront.jpg"
        
    };

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int width, height, nChannels;
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            cerr << "unable to load " << faces[i] << endl;
            stbi_image_free(data);
            exit(1);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    

    return texID;
}

void Manager::UpdateNoiseMesh(){
    //TODO: get perlin noise data here
    int controlRows = 35;
    //cerr << "running update noise mesh" << endl;
    //loop through and make new points
    pointGrid = vector<vector<vec3>>(controlRows, vector<vec3>(controlRows, vec3(0.0f))); //clear the pointgrid and make it all (0,0,0).
    float debugYmax = -FLT_MAX, debugYmin = FLT_MAX;
    float debugXmax = -FLT_MAX, debugXmin = FLT_MAX;
    float debugZmax = -FLT_MAX, debugZmin = FLT_MAX;
    for(int x = 0; x < controlRows; x++){
        for(int y = 0; y < controlRows; y++){
            //add perlin height to gridpoints
            float randomX = ((float)rand()) / (float)RAND_MAX;
            float randomY = ((float)rand()) / (float)RAND_MAX;
			float randomZ = ((float)rand()) / (float)RAND_MAX;
            //set the scale here so we can 
            pointGrid[x][y] = vec3((float)x + randomX * 1.0f - 0.5f, 0.0f, (float)y + randomZ * 1.0f - 0.5f);
            
            
            
            pointGrid[x][y].y = randomY * 3.0f - 1.5f;

            pointGrid[x][y].x *= 100.0f; //scale here so we can coincide our helicopter's position with a position on the mesh!
            pointGrid[x][y].z *= 100.0f;
            pointGrid[x][y].y *= 30.0f;

            //only check the grid points, as every value on the splines will be something inbetween those.
            if (pointGrid[x][y].y < debugYmin) {
                debugYmin = pointGrid[x][y].y;
            }

			if (pointGrid[x][y].y > debugYmax) {
                debugYmax = pointGrid[x][y].y;
			}
            

			if (pointGrid[x][y].x < debugXmin) {
                debugXmin = pointGrid[x][y].x;
			}

			if (pointGrid[x][y].x > debugXmax) {
                debugXmax = pointGrid[x][y].x;
			}

			if (pointGrid[x][y].z < debugZmin) {
                debugZmin = pointGrid[x][y].z;
			}

			if (pointGrid[x][y].z > debugZmax) {
				debugZmax = pointGrid[x][y].z;
			}
        }
    }

    cerr << endl << "X x Y x Z: " << endl << "[" << debugXmin << ", " << debugXmax << "] x [" << debugYmin << ", " << debugYmax <<
        "] x [" << debugZmin << ", " << debugZmax << "]" << endl;

    bool createIndBuf = false;
    //create new mesh
    if(indBuf.size() == 0){
        //need to make the index buffer!
        createIndBuf = true;
    }


    //make triangles with the INNER portion, starting from bottom left
    //this may literally be a quadruple loop so ill make a separate function for it
    int indexOffset = 0;
    for(int x = 1; x <= controlRows - 3; x++){
        for(int y = 1; y <= controlRows - 3; y++){
            indexOffset = CreateTrianglesForSurface(x,y, indexOffset, createIndBuf);
        }
    }

    noiseMesh = make_shared<Shape>();
    noiseMesh->CopyData(posBuf, norBuf);
    noiseMesh->init();
}


vector<vector<vec3>> Manager::GenerateControlPoints(int baseX, int baseY){
    vector<vector<vec3>> ret;
    for(int x = baseX - 1; x <= baseX + 2; x++){
        //cerr << "X: " << x << endl;
        vector<vec3> curr;
        for(int y = baseY - 1; y <= baseY + 2; y++){
            curr.push_back(pointGrid[x][y]);
            //cerr << pointGrid[x][y].x << ", " << pointGrid[x][y].y << ", " << pointGrid[x][y].z << endl;
            //cerr << "point " << x << ", " << y << endl;
        }
        //cerr << endl << endl;
        ret.push_back(curr);
        curr.clear();
    }
    return ret;
}

vec3 Manager::CalculatePoint(vector<vector<vec3>>& cpoints, int x, int y, float u, float v){
	assert(cpoints.size() == 4 && cpoints[0].size() == 4);

    vec4 uVec = vec4(1, u, (float)u*u, (float)u*u*u);
    vec4 vVec = vec4(1, v, (float)v*v, (float)v*v*v);
    
    
    //need to make Gx, Gy, Gz.
    mat4 Gx(0.0f);
    mat4 Gy(0.0f);
    mat4 Gz(0.0f);
    //cerr << "points: " << endl;
    for(int i = 0; i < 4; i++){
        Gx[i] = vec4(cpoints[0][i].x, cpoints[1][i].x, cpoints[2][i].x, cpoints[3][i].x);
        Gy[i] = vec4(cpoints[0][i].y, cpoints[1][i].y, cpoints[2][i].y, cpoints[3][i].y);
        Gz[i] = vec4(cpoints[0][i].z, cpoints[1][i].z, cpoints[2][i].z, cpoints[3][i].z);

        

        /*
        cerr << cpoints[0][i].x << ", " << cpoints[0][i].y << ", " << cpoints[0][i].z << endl;
        cerr << cpoints[1][i].x << ", " << cpoints[1][i].y << ", " << cpoints[1][i].z << endl;
        cerr << cpoints[2][i].x << ", " << cpoints[2][i].y << ", " << cpoints[2][i].z << endl;
        cerr << cpoints[3][i].x << ", " << cpoints[3][i].y << ", " << cpoints[3][i].z << endl;
        */
    }
    //cerr << endl;

    vec4 Bu = B * uVec;
    vec4 Bv = B * vVec;

    vec4 GxBu = Gx * Bu;
    vec4 GyBu = Gy * Bu;
    vec4 GzBu = Gz * Bu;

    float Px = dot(Bv, GxBu);
    float Py = dot(Bv, GyBu);
    float Pz = dot(Bv, GzBu);

    

    //cerr << "point: " << Px << ", " << Py << ", " << Pz << endl;
    return vec3(Px, Py, Pz);
}


int Manager::CreateTrianglesForSurface(int x, int y, int indexOffset, bool isCreateInd){
    //cerr << "making triangles for the mesh @ " << x << ", " << y << endl;
    vector<vector<vec3>> Points = GenerateControlPoints(x,y);
    
    for(float u = 0.0f; u < 1.0f; u+=0.1f){
        for(float v = 0.0f; v < 1.0f; v+=0.1f){
            //Calculating the 4 points in the square
            vec3 bot_left = CalculatePoint(Points, x, y, u, v);
            vec3 bot_right = CalculatePoint(Points, x, y, u + 0.1f, v);
            vec3 top_left = CalculatePoint(Points, x, y, u, v + 0.1f);
            vec3 top_right = CalculatePoint(Points, x, y, u + 0.1f, v + 0.1f);
            
            //cerr << "FOR u = " << x + u << " AND v = " << y + v << endl;
            //cerr << "square:\n" << top_left.x << ", " << top_left.y << ", " << top_left.z << "\t -- \t" << top_right.x << ", " << top_right.y << ", " << top_right.z << endl;
            //cerr << bot_left.x << ", " << bot_left.y << ", " << bot_left.z << "\t -- \t" << bot_right.x << ", " << bot_right.y << ", " << bot_right.z << endl << endl;
            

            //assert(false);
            //tri 1
            posBuf.push_back(bot_left.x);
            posBuf.push_back(bot_left.y);
            posBuf.push_back(bot_left.z);

			posBuf.push_back(bot_right.x);
			posBuf.push_back(bot_right.y);
			posBuf.push_back(bot_right.z);

			posBuf.push_back(top_left.x);
			posBuf.push_back(top_left.y);
			posBuf.push_back(top_left.z);

            
            ///tri 2
            
			posBuf.push_back(top_left.x);
			posBuf.push_back(top_left.y);
			posBuf.push_back(top_left.z);

			posBuf.push_back(bot_right.x);
			posBuf.push_back(bot_right.y);
			posBuf.push_back(bot_right.z);

			posBuf.push_back(top_right.x);
			posBuf.push_back(top_right.y);
			posBuf.push_back(top_right.z);
            

            

            //tri 1
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);
            
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);
            
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);

            //tri 2
            
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);
            
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);
            
            norBuf.push_back(0.0f);
            norBuf.push_back(1.0f);
            norBuf.push_back(0.0f);
            

        }
    }

    return indexOffset;
}
