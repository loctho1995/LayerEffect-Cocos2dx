#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class Bloom
{
public:
    static void bloom(RenderTexture *texture, Texture2D *highLight);

protected:
    Bloom();
    ~Bloom();

    static void initShaders();

    static bool isInitShader;
};