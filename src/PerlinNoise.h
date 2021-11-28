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
    std::vector<int> Perm; //permutation, to be shuffled

    PerlinNoise();
    PerlinNoise(int _sz); //noise will be square
    ~PerlinNoise();
    
    void ThreadSetNoise(float x, float y, float& minY, float& maxY);

    //make perm vector with shuffling
    std::vector<int> MakePerm();
    void ShuffleVec(std::vector<int>& P);

    glm::vec2 GetConstantVector(int p);
    float Noise2D(float x, float y);

    float Fade(float t);
    float Lerp(float A, float B, float t);

    void PrintNoiseData();


    private:

};