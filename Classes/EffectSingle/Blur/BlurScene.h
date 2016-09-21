#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class BlurScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void update(float dt);

    bool onTouchBegan(Touch *touch, Event *e);
    void onTouchMoved(Touch *touch, Event *e);
    void onTouchEnded(Touch *touch, Event *e);    

    CREATE_FUNC(BlurScene);

private:
    Sprite *sprite;
    RenderTexture *texture;

    int count, count2;
};
