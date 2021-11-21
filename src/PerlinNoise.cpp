#include "PerlinNoise.h"
#include <fstream>

using namespace std;

PerlinNoise::PerlinNoise(){
    sz = 20;
    noiseData = vector<vector<float> >(sz, vector<float>(sz, 0.0f));
}

PerlinNoise::PerlinNoise(int _sz, string DataFile){
    sz = _sz;
    noiseData = vector<vector<float> >(sz, vector<float>(sz, 0.0f));

    fstream f;

    f.open(DataFile);

    //READ DATA

    f.close();
}

PerlinNoise::~PerlinNoise(){

}