#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>

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

//UI Manager for rendering on the HUD
class UIManager{
    public:
        #pragma region SingletonImplementation
        static UIManager &Instance()
        {
            static UIManager m_instance; //properly instantiated on first use, destroyed on 2nd and after.
            return m_instance;
        }

        private:
        UIManager() {}

        public:
        //ensure that no copies ever get made.
        UIManager(UIManager const &) = delete;
        void operator=(UIManager const &) = delete;
        #pragma endregion

        //logistical stuff
		GLFWwindow* window;
		std::string RESOURCE_DIR = ""; // Where the resources are loaded from


        //data to be loaded
        std::vector< std::shared_ptr<Texture> > digits;

        std::shared_ptr<Texture> remainingTex; //need to make a rectangle...?

        std::shared_ptr<Texture> reticle;

        std::shared_ptr<Shape> quad;

        std::shared_ptr<Program> textureProg;

        //posBuf
        GLuint posBufID;
        std::vector<float> posBuf;

        //values for displaying text
        int remainingTargets = 99;

        void init(GLFWwindow* _win, std::string& rez);

        void renderUI();


        //helpers
        void DecrementTargets() { remainingTargets--; }
};