#include "PerlinNoise.h"
#include "stb_image.h"

#include <iomanip>
#include <thread>

using namespace std;
using namespace glm;

//adapted from (tutorial name)

PerlinNoise::PerlinNoise(){
    sz = 20;
    noiseData = vector<vector<float> >(sz, vector<float>(sz, 0.0f));
}

PerlinNoise::PerlinNoise(int _sz){
    sz = _sz;
    noiseData = vector<vector<float> >(sz, vector<float>(sz, 0.0f));
    srand(time(NULL));
    //generating this mess by ourselves
    minY = FLT_MAX, maxY = -FLT_MAX;
    for (int y = 0; y < sz; y++) {
        for (int x = 0; x < sz; x++) {
            //TODO: thread this too.
            ThreadSetNoise(x, y, minY, maxY);
        }
    }

    //remap everything to [0.0, 1.0] for ease of shading!
    float diff = maxY - minY;
    for (int x = 0; x < sz; x++) {
        for (int y = 0; y < sz; y++) {
            float mappedData = noiseData[x][y] - minY;
            noiseData[x][y] = mappedData / diff;
        }
    }

    //create image of the noise to test
    //PrintNoiseData(); //at least confirm its not all 1s
}

PerlinNoise::~PerlinNoise(){

}

void PerlinNoise::ThreadSetNoise(float x, float y, float& minY, float& maxY) {
    float n = Noise2D(0.01f * x, 0.01f * y); //[-1.0, 1.0]
    noiseData[y][x] = n;

    minY = __min(minY, n);
    maxY = __max(maxY, n);
}

vector<int> PerlinNoise::MakePerm()
{
    vector<int> ret;
    ret.clear();
    ret.resize(sz);
	for (int i = 0; i < sz; i++) {
        ret[i] = i;
	}
	ShuffleVec(ret);
    for (int i = 0; i < sz; i++) {
        
        ret.push_back(ret[ret[i]]);
    }
    return ret;
}

void PerlinNoise::ShuffleVec(std::vector<int>& P)
{
    //shuffle algo
    assert(P.size() == sz);
    for (int k = sz - 1; k > 0; k--) {
        int ind = rand() % (k + 1);
        if (k == ind || ind < 0) {
            continue; 
        }

        int temp = P[k];
        P[k] = P[ind];
        P[ind] = temp;
    }
}

glm::vec2 PerlinNoise::GetConstantVector(int p)
{
    int h = p % 4;
    vec2 ret;
    switch (h) {
        case 0:
            ret = vec2(1.0f, 1.0f);
            break;
        case 1:
            ret = vec2(-1.0f, 1.0f);
            break;
        case 2:
            ret = vec2(-1.0f, -1.0f);
            break;
        case 3:
            ret = vec2(1.0f, -1.0f);
            break;
        default:
            cerr << "received value " << h << " in GetConstantVector! exiting..." << endl;
            exit(1);
    }

    return ret;
}

float PerlinNoise::Noise2D(float x, float y)
{
    //integer parts
    int Xi = (int)floor(x);
    int Yi = (int)floor(y);

    //float parts
    float Xf = x - Xi;
    float Yf = y - Yi;

    //square points - the tutorial does the direction for these really weird.
    vec2 TR(Xf - 1.0f, Yf - 1.0f);
    vec2 TL(Xf, Yf - 1.0f);
    vec2 BR(Xf - 1.0f, Yf);
    vec2 BL(Xf, Yf);

    Perm = MakePerm();

    int TR_val = Perm[Perm[Xi + 1] + Yi + 1];
    int TL_val = Perm[Perm[Xi] + Yi + 1];
    int BR_val = Perm[Perm[Xi + 1] + Yi];
    int BL_val = Perm[Perm[Xi] + Yi];

    
    float TRdot = dot(TR, GetConstantVector(TR_val));
    float TLdot = dot(TL, GetConstantVector(TL_val));
    float BRdot = dot(BR, GetConstantVector(BR_val));
    float BLdot = dot(BL, GetConstantVector(BL_val));

    float u = Fade(Xf); //x lerp value
    float v = Fade(Yf); //y lerp value

    float left = Lerp(v, BLdot, TLdot);
    float right = Lerp(v, BRdot, TRdot);
    return Lerp(u, left, right);
}

float PerlinNoise::Fade(float t)
{
    //example uses 6t^5 - 15t^4 + 10t^3, here's a simplified version
    return pow(t, 3) * (6.0f * pow(t, 2) - 15.0f * t + 10.0f);
}

float PerlinNoise::Lerp(float A, float B, float t)
{
    return t * A + (1.0 - t) * B;
}

void PerlinNoise::PrintNoiseData() {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < sz; j++) {
            cout << fixed << setprecision(2) << noiseData[i][j] << " ";
        }
        cout << "\n\n";
    }

    exit(0); //dont particularly care past this point at the moment
}
