#pragma once
#include <iostream>
#include <vector>
#include <map>

#include "MatrixStack.h"
#include "Shape.h"
#include "Program.h"
#include "Camera.h"
#include "GLSL.h"

class PerlinNoise{
    public:
    int sz; //square size of the FULL noise
    std::vector<std::vector<float>> noiseData; //noise data, should be sz x sz.

    PerlinNoise();
    PerlinNoise(int _sz, std::string DataFile);
    ~PerlinNoise();
    


    private:

};