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
#include "Point.h"

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
};