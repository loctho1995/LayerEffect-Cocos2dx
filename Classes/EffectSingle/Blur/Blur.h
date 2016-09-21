#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class Blur
{
public:
    static Sprite* blurSprite(const char* fileName, int radius);
    static void blur(RenderTexture *texture, int radius);
    static void horizontalBlur(RenderTexture *texture, int radius);
    static void VerticalBlur(RenderTexture *texture, int radius);
    static void blurKernel9(RenderTexture *texture, int radius);
    static void gaussianBlur(RenderTexture *texture, Size pixelSize, Vec2 direction, int radius);
        
protected:
    Blur();
    ~Blur();

    static void calculateGaussianWeights(const int points, float* weights);

    static void initShaders();

    static bool isInitShader;

    static CustomCommand command;
};