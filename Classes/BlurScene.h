#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"
#include "SpriteBloom.h"
#include "LayerEffect.h"
#include "SpriteAmbientLight.h"
#include "LabelSlider.h"

using namespace cocos2d::extension;
using namespace cocos2d::ui;
using namespace cocos2d;

#define SLIDER_RADIUS_TAG 1
#define SLIDER_CONSTRACT_TAG 2
#define SLIDER_THRESHOLD_TAG 3
#define SLIDER_BRIGHTNESS_TAG 4

#define SPRITE_ALLOW_CONTROL_TAG 111

class BlurScene : public LayerEffect
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    virtual void update(float dt);

	virtual void onUpdateUniform();

	void createLightIcon(Vec2 position, int key);

    bool onTouchBegan(Touch *touch, Event *e);
    void onTouchMoved(Touch *touch, Event *e);
    void onTouchEnded(Touch *touch, Event *e);   

	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	void valueSliderChanged(Ref *sender, Control::EventType controlEvent);

	void colourValueChanged(Ref *sender, extension::Control::EventType controlEvent);

	void createSliders();

	void addTouchEvent();

	void createEffectNode();

	void createColorPicker();

	void onTouchBackground();

	void createSpriteControlable(const char* fileName, Vec2 position);

	void createSpriteBloom(const char* fileName, Vec2 position, float radius = 7.0f, float constract = 1.0, float threshold = 0.0);

	void onSpriteBloomTouched();

	void onChildEffectTouched();

    CREATE_FUNC(BlurScene);

private:
	void onSpriteChanged();

	LabelSlider *sliderRadius, *sliderBrightness, *sliderThreshold;
    Sprite *originalSprite, *showSprite, *choosenBox;
	SpriteBloom *spriteBloom;
	Node *choosenNode;
	Size visibleSize;

	ControlColourPicker *colorPicker;

	CustomCommand command;

	AmbientLightParam light;
	SpriteAmbientLight *ambientLight;

	std::map<int, Sprite*> icons;

	//effectIndex index cua effect dang duoc chon
	//childIndex index cua child dang duoc chon
	int effectIndex, childIndex;

    float radiusCount, deltaRadius, rangeRadius;
    float distortionCount, distortionDelta;

    bool isUpdated, isValueChangeFromTouch, isKeyDone;
};
