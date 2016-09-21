#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class BrightFilter
{
public:
    static Sprite* brightFilterSprite(const char* fileName);
    static void brightFilter(RenderTexture *texture);

protected:
    BrightFilter();
    ~BrightFilter();

    static void initShaders();

    static bool isInitShader;

    static CustomCommand command;
};
