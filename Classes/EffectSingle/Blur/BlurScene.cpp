#include "BlurScene.h"
//#include "Blur.h"
//#include "BrightFilter\BrightFilter.h"
//#include "Bloom\Bloom.h"
//#include "EffectNode.h"
#include "EffectCenter.h"

Scene* BlurScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = BlurScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BlurScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size size = Director::getInstance()->getVisibleSize();

    //EffectNode *node = EffectNode::create();
    //node->setPosition(size.width / 2, size.height / 2);
    //this->addChild(node);

    //auto sprite = BrightFilter::brightFilterSprite("owlimage.png");//Blur::blurSprite("owlimage.png", 1);
    //sprite->setPosition(size.width / 2, size.height / 2);
    //this->addChild(sprite);

    auto sprite = Sprite::create("sox.png");
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

    //auto brightFilterShader = GLProgram::createWithFilenames("shaders/BrightFilter/brightFilter.vert", "shaders/BrightFilter/brightFilter.frag");
    //sprite->setGLProgram(brightFilterShader);

    texture = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    
    texture->begin();
    sprite->visit();
    texture->end();
    Director::getInstance()->getRenderer()->render();        

    texture->setPosition(size.width / 2, size.height / 2);
    this->addChild(texture);
        
    EffectCenter::bloom(texture, 4.0);
    //EffectCenter::brightFilter(texture);
    //Blur::blur(texture, 5);
        
    /*int step = 1;    
    Size textureSize = texture->getSprite()->getContentSize();
    int radius = 5;
    Size pixelSize = Size(float(step) / textureSize.width, float(step) / textureSize.height);
    int radiusWithStep = radius / step;

    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, 0.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(-1.0, 0.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(0.0, 1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(0.0, -1.0), radiusWithStep);

    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, 1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(-1.0, 1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, -1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(-1.0, -1.0), radiusWithStep);*/
    
    /*sprite = Sprite::create("sox.png");
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

    RenderTexture *r2 = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    r2->begin();
    sprite->visit();
    r2->end();
    Director::getInstance()->getRenderer()->render();

    Bloom::bloom(texture, r2->getSprite()->getTexture());

    auto sprite2 = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite2->setFlipY(true);
    sprite2->setPosition(size.width / 2, size.height / 2);
    this->addChild(sprite2);
    */
    count = 0;
    count2 = 0;

    this->scheduleUpdate();

    //this->addChild(sprite);
    //texture->retain();
    return true;
}

void BlurScene::update(float dt)
{
    if (count >= 60)
    {
        count = 0;

        //Blur::blur(texture, 5);
    }
    else
    {
        count++;
    }
    
    return;
    if (count >= 60)
    {
        //Blur::blur(texture, 1);
        //count = 0;

        if (count2 == 3)
        {
            //Blur::blur(texture, 1);

            //BrightFilter::brightFilter(texture);
            /*Size size = Director::getInstance()->getVisibleSize();
            sprite = Sprite::create("sox.png");
            sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

            RenderTexture *r2 = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
            r2->begin();
            sprite->visit();
            r2->end();
            Director::getInstance()->getRenderer()->render();

            Bloom::bloom(texture, r2->getSprite()->getTexture());

            auto sprite2 = Sprite::createWithTexture(texture->getSprite()->getTexture());
            sprite2->setFlipY(true);
            sprite2->setPosition(size.width / 2, size.height / 2);
            this->addChild(sprite2);
            count2 = 5;*/
        }
        else if (count2 < 3)
        {
            //CCLOG("fuck");
            //count2++;
            //count = 0;
            //Blur::blur(texture, 1);

            //int step = 1;
            //Size textureSize = texture->getSprite()->getContentSize();
            //int radius = 30;
            //Size pixelSize = Size(float(step) / textureSize.width, float(step) / textureSize.height);
            //int radiusWithStep = radius / step;
            //Blur::gaussianBlur(texture, pixelSize, Vec2(1.0, 0.0), radiusWithStep);
            //Blur::gaussianBlur(texture, pixelSize, Vec2(0.0, 1.0), radiusWithStep);
        }
    }
    else
    {
        count++;
    }

    //CCLOG("time: %d", count2);
}

bool BlurScene::onTouchBegan(Touch *touch, Event *e)
{
    return true;
}

void BlurScene::onTouchMoved(Touch *touch, Event *e)
{

}

void BlurScene::onTouchEnded(Touch *touch, Event *e)
{    

}