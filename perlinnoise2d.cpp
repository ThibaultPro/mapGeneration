#include "perlinnoise2d.h"


PerlinNoise2d::PerlinNoise2d(int nWidth, int nHeight)
{
    nOutputWidth = nWidth;
    nOutputHeight = nHeight;
    fNoiseSeed2D = new float[nOutputWidth * nOutputHeight];
    fPerlinNoise2D = new float[nOutputWidth * nOutputHeight];
    for (int i = 0; i < nOutputWidth * nOutputHeight; i++) fNoiseSeed2D[i] = (float)rand()/ (float)RAND_MAX;

}

void PerlinNoise2d::generatePerlinNoiseArray(int nOctaveCount, float fScalingBias)
{
    for (int x = 0; x < nOutputWidth; x++)
        for (int y = 0; y < nOutputHeight; y++)
        {
            float fNoise = 0.0f;
            float fScaleAcc = 0.0f;
            float fScale = 1.0f;

            for (int o = 0; o < nOctaveCount; o++)
            {
                int nPitch = nOutputWidth >> o;
                int nSampleX1 = (x / nPitch) * nPitch;
                int nSampleY1 = (y / nPitch) * nPitch;

                int nSampleX2 = (nSampleX1 + nPitch) % nOutputWidth;
                int nSampleY2 = (nSampleY1 + nPitch) % nOutputWidth;

                float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
                float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

                float fSampleT = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY1 * nOutputWidth + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY1 * nOutputWidth + nSampleX2];
                float fSampleB = (1.0f - fBlendX) * fNoiseSeed2D[nSampleY2 * nOutputWidth + nSampleX1] + fBlendX * fNoiseSeed2D[nSampleY2 * nOutputWidth + nSampleX2];

                fScaleAcc += fScale;
                fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
                fScale = fScale / fScalingBias;
            }

            // Scale to seed range
            fPerlinNoise2D[y * nOutputWidth + x] = fNoise / fScaleAcc;

        }
}
