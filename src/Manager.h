#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <thread>
#include <chrono>
#include <mutex>
#include <semaphore>

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
#include "PerlinNoise.h"
#include "ParticleSystem.h"
#include "Texture.h"

class Missile;
class Target;

//General manager for all game things that need to go between files
//^comment written before i did everything in manager lmao
class Manager{
    public:
        #pragma region SingletonImplementation
        static Manager &Instance()
        {
            static Manager m_instance; //properly instantiated on first use, destroyed on 2nd and after.
            return m_instance;
        }

        private:
        Manager() {}

        public:
        //ensure that no copies ever get made.
        Manager(Manager const &) = delete;
        void operator=(Manager const &) = delete;
        #pragma endregion

        #pragma region Logistics
        GLFWwindow* window;
        std::string RESOURCE_DIR = ""; // Where the resources are loaded from

        int keyPresses[256] = {0}; // only for English keyboards!

        bool gameWon = false;
        float winTimer = 0.0f;

        std::shared_ptr<Camera> camera;
        //shaders
        std::shared_ptr<Program> prog;
        std::shared_ptr<Program> worldprog;
        std::shared_ptr<Program> skyboxprog;
        std::shared_ptr<Program> quadPS;

        bool inMeshCreatorMode = true; //are we in the mesh creation mode?
        bool initTrigger = false; //when true, we do the respective init() function for the scene we're in, then set this back to false.

        void RenderLoadingScreen();
        void InitializeShaders();
        void init_meshcreator();
        void render_meshcreator();

        void init_helicopter();
        void render_helicopter();

        void GameLoop(GLFWwindow* window);
        
        #pragma endregion

        unsigned int LoadCubeMap();
        unsigned int skyboxID;

        #pragma region Mesh
        //buffers for the procedural mesh
        std::vector<float> posBuf;
        std::vector<float> norBuf;
        std::vector<int> indBuf;

        std::shared_ptr<Shape> noiseMesh;
        std::shared_ptr<Shape> skybox;

        std::vector<std::vector<glm::vec3>> pointGrid; //shoot for 100x100 at a very large spline interval
        std::vector<std::vector<float>> scales;
        std::shared_ptr<PerlinNoise> perlin;
        float XZscale = 45.0f;
        float Yscale = 15.0f;
        float meshMinY = 0.0f;
        float meshMaxY = 0.0f;

        std::pair<float, float> meshXBounds = std::make_pair(0.0f, 0.0f);
        std::pair<float, float> meshZBounds = std::make_pair(0.0f, 0.0f);

        glm::mat4 B; //basis for (i believe) B-spline?

        std::set<std::pair<float, float>> debugSet;

        int rowsPerThread = 125;
        std::vector<std::thread> meshWorkers;
        std::mutex mx;
        
        int controlRows = 53;
        void UpdateNoiseMesh();
        //0,0 = bottom left, 3,3 = top right
        std::vector<std::vector<glm::vec3>> GenerateControlPoints(int baseX, int baseY);
        glm::vec3 CalculatePoint(std::vector<std::vector<glm::vec3>>& cpoints, int x, int y, float u, float v);
        int CreateTrianglesForSurface(int x, int y, int indexOffset, bool isCreateInd);

        #pragma endregion  

        #pragma region Helicopter
        float deltaXRot = 0.0f;
        float deltaYRot = 0.0f;
        float deltaZRot = 0.0f;
        float deltaSpeed = 0.0f;

        float pitch_mag = 0.0f;
        float yaw_mag = 0.0f;
        float roll_mag = 0.0f;
        float speed_mag = 0.0f;

        void CalculateTurnAcceleration(float _x, float _y, float _z, float _s);
        void StorePreviousDirection(float _x,float _y, float _z, float _s);
        void UpdateRotation(float _x, float _y, float _z, float _s);
        void DrawHelicopter(std::shared_ptr<MatrixStack> P, std::shared_ptr<MatrixStack> MV, std::shared_ptr<Program> prog, double t);
        void CheckForEnvironmentCollision();
        void PlayerDeath();
        
        void DebugRKey();
        #pragma endregion

        #pragma region Missile
        void FireMissile();
        void DestroyMissile();
        
        bool missileActive = false;
        std::shared_ptr<Missile> missile;

        int ms_index = 0;
        std::vector< std::shared_ptr<Missile> > missileVec;
        std::vector< bool > ms_active;
        std::vector< std::shared_ptr<Target> > targets;

        void GenerateTargets(); // will add to "targets"
        #pragma endregion
};
