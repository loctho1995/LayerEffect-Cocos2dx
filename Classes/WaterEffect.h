#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class WaterEffect
{
public:
    WaterEffect();
    ~WaterEffect();

    static void water(RenderTexture *texture);
};

