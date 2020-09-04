#ifndef PERLINNOISE2D_H
#define PERLINNOISE2D_H

#include <iostream>
#include <string>
#include <algorithm>
#include <QLabel>

using namespace std;

class PerlinNoise2d
{
public:
    PerlinNoise2d(int nOutputWidth, int nOutputHeight);
    void generatePerlinNoiseArray(int nOctaveCount, float fScalingBias);

private:
    float *fNoiseSeed2D;
    float *fPerlinNoise2D;
    int nOutputWidth;
    int nOutputHeight;
};

#endif // PERLINNOISE2D_H
