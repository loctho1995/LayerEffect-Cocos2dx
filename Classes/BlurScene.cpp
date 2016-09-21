#include "BlurScene.h"
#include "AreaLighting.h"
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
    if ( !LayerEffect::init() )
    {
        return false;
    }

	/*auto p = GLProgram::createWithFilenames("shaders/Lighting/spotLight.vert", "shaders/Lighting/spotLight.frag");*/
	//auto p = GLProgram::createWithFilenames("shaders/ambientLight.vert", "shaders/ambientLight.frag");
	//this->setGLProgram(p);

	childIndex = -1;
	effectIndex = -1;
	Size size = Director::getInstance()->getVisibleSize();
	visibleSize = size;
	Vec2 center = Vec2(size.width / 2, size.height / 2);

	this->setContentSize(Size(size.width, size.height));

	//auto backGround = SpriteBloom::createSpriteBloom("background.png", 2.0f);
	//backGround->setPosition(size.width / 2, size.height / 2);
	//this->addChild(backGround);

	choosenNode = nullptr;

	//auto background = Sprite::create("background.png");
	//background->setPosition(size.width / 2, size.height / 2);
	//this->addChild(background);

	float delta = 300;
	
	//createSpriteControlable("tree1.png", Vec2(200, size.height / 2));
	//createSpriteControlable("tree2.png", Vec2(delta * 2, size.height / 2));
	//createSpriteControlable("tree3.png", Vec2(delta * 3, size.height / 2));
	//createSpriteControlable("tree4.png", Vec2(delta * 4, size.height / 2));

	float scale = 0.8;

	auto sprite = Sprite::create("demo/Background1.png");
	sprite->setPosition(size.width / 2, size.height / 2);
	sprite->setScale(scale);
	this->addChild(sprite);

	auto sprite2 = SpriteBloom::createSpriteBloom("demo/Background2.png", 2.0, 1.0, 0);
	sprite2->setPosition(sprite->getPosition());
	sprite2->setScale(scale);
	sprite2->setTag(SPRITE_ALLOW_CONTROL_TAG);
	this->addChild(sprite2);

	auto sprite3 = Sprite::create("demo/ColorDeep.png");
	sprite3->setPosition(size.width / 2, size.height / 2);
	sprite3->setScale(scale);
	sprite3->setOpacity(100);
	this->addChild(sprite3);

	auto sprite4 = SpriteBloom::createSpriteBloom("demo/Foreground.png", 6.0, 1.0, 0.0);
	sprite4->setPosition(sprite->getPosition());
	sprite4->setScale(scale);
	sprite4->setTag(SPRITE_ALLOW_CONTROL_TAG);
	this->addChild(sprite4);

	auto sprite5 = Sprite::create("demo/Playground.png");
	sprite5->setPosition(sprite->getPosition());
	sprite5->setScale(scale);
	this->addChild(sprite5);

	//createSpriteBloom("ori.png", Vec2(size.width / 2, size.height / 2), 7.0, 1.0, 0.5f);
	//createSpriteBloom("tree2.png", Vec2(delta * 2, size.height / 2), 7.0, 1.0, 0.5f);
	//createSpriteBloom("tree3.png", Vec2(delta * 3, size.height / 2), 7.0, 1.0, 0.5f);
	//createSpriteBloom("tree4.png", Vec2(delta * 4, size.height / 2), 7.0, 1.0, 0.5f);
	createEffectNode();

	addTouchEvent();

	createSliders();

	createColorPicker();

	this->scheduleUpdate();

	this->setKeyboardEnabled(true);

    return true;
}

void BlurScene::update(float dt)
{
    //EffectCenter::bloom(texture, radiusCount, 0.5, 1.2);

    radiusCount += deltaRadius;

    //distortionCount += distortionDelta;
    //EffectCenter::distortion(texture, distortionCount, 0);

    //if (distortionCount >= 0.02)
    //{
    //    distortionDelta = -distortionDelta;
    //}
    //else if (distortionCount <= -0.02)
    //{
    //    distortionDelta = -distortionDelta;
    //}

    //EffectCenter::distortion(texture, sin(radiusCount) / 100.0f, cos(radiusCount) / 100.0f);
    //EffectCenter::distortion(texture, radiusCount + 5.0f, 5.0f);

    if (radiusCount >= rangeRadius && deltaRadius > 0)
    {
        deltaRadius = -deltaRadius;
    }
    else if (radiusCount <= 2.0 && deltaRadius < 0)
    {
        deltaRadius = abs(deltaRadius);
    }
}

void BlurScene::addTouchEvent()
{
	auto touchEvent = EventListenerTouchOneByOne::create();
	touchEvent->onTouchBegan = CC_CALLBACK_2(BlurScene::onTouchBegan, this);
	touchEvent->onTouchMoved = CC_CALLBACK_2(BlurScene::onTouchMoved, this);
	touchEvent->onTouchEnded = CC_CALLBACK_2(BlurScene::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

	auto keyboardEvent = EventListenerKeyboard::create();
	keyboardEvent->onKeyPressed = CC_CALLBACK_2(BlurScene::onKeyPressed, this);
	keyboardEvent->onKeyReleased = CC_CALLBACK_2(BlurScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardEvent, this);
}

void BlurScene::createSpriteControlable(const char* fileName, Vec2 position)
{
	auto sprite = Sprite::create(fileName);
	sprite->setPosition(position);
	sprite->setTag(SPRITE_ALLOW_CONTROL_TAG);
	this->addChild(sprite);
}

void BlurScene::createSpriteBloom(const char* fileName, Vec2 position, 
										float radius, float constract, float threshold)
{
	spriteBloom = SpriteBloom::createSpriteBloom(fileName, radius, constract, threshold);
	spriteBloom->setTag(SPRITE_ALLOW_CONTROL_TAG);
	spriteBloom->setPosition(position);
	spriteBloom->Node::setContentSize(spriteBloom->getContentSize());
	this->addChild(spriteBloom);
}

void BlurScene::createColorPicker()
{
	colorPicker = ControlColourPicker::create();
	colorPicker->setAnchorPoint(Vec2(0.0, 0.5));
	colorPicker->setPosition(visibleSize.width - colorPicker->getContentSize().width / 2, visibleSize.height / 2);
	colorPicker->setScale(0.7);
	colorPicker->setVisible(false);
	colorPicker->setEnabled(false);
	colorPicker->addTargetWithActionForControlEvents(this, cccontrol_selector(BlurScene::colourValueChanged), Control::EventType::VALUE_CHANGED);
	this->addChildWithoutEffect(colorPicker);
}

void BlurScene::createSliders()
{
	float sliderScale = 0.5f;
	float fontSize = 14;
	float height = 60;

	sliderRadius = LabelSlider::create("Radius", 1.0, 2000.0);	
	sliderRadius->setPosition(visibleSize.width - 30.0, visibleSize.height - 200);
	sliderRadius->addTargetWithActionForControlEvents(this, cccontrol_selector(BlurScene::valueSliderChanged));
	sliderRadius->setVisible(false);
	sliderRadius->setMask(SLIDER_RADIUS_TAG);
	sliderRadius->setAnchorPoint(Vec2(1.0, 0.5f));
	this->addChildWithoutEffect(sliderRadius);

	sliderBrightness = LabelSlider::create("Brightness", 0.0, 10.0);
	sliderBrightness->setPosition(sliderRadius->getPosition() + Vec2(0, height));
	sliderBrightness->addTargetWithActionForControlEvents(this, cccontrol_selector(BlurScene::valueSliderChanged));
	sliderBrightness->setVisible(false);
	sliderBrightness->setMask(SLIDER_BRIGHTNESS_TAG);
	sliderBrightness->setAnchorPoint(Vec2(1.0, 0.5f));
	this->addChildWithoutEffect(sliderBrightness);

	sliderThreshold = LabelSlider::create("Threshold", 0.0, 1.0);
	sliderThreshold->setPosition(sliderBrightness->getPosition() + Vec2(0, height));
	sliderThreshold->addTargetWithActionForControlEvents(this, cccontrol_selector(BlurScene::valueSliderChanged));
	sliderThreshold->setVisible(false);
	sliderThreshold->setMask(SLIDER_THRESHOLD_TAG);
	sliderThreshold->setAnchorPoint(Vec2(1.0, 0.5f));
	this->addChildWithoutEffect(sliderThreshold);
}

void BlurScene::createEffectNode()
{
	Vec2 center = Vec2(visibleSize.width / 2, visibleSize.height / 2);

	ambientLight = SpriteAmbientLight::create(center, 350, Color4F::WHITE, 0.2);
	this->addChildEffect(ambientLight);
	createLightIcon(center, 0);

	srand(time(NULL));
	for (size_t i = 0; i < 10; i++)
	{
		Vec2 pos = Vec2(150 * i, center.y);
		Color4F color = Color4F(Color3B(rand() % 255, rand() % 255, rand() % 255));

		auto node = SpriteAmbientLight::create(pos, 150, color, 0.3);

		this->addChildEffect(node);

		createLightIcon(node->getPosition(), childrenEffect.size() - 1);
	}
}

void BlurScene::createLightIcon(Vec2 position, int key)
{
	auto sprite = Sprite::create("lightIcon.png");
	sprite->setPosition(position);
	sprite->setOpacity(150);
	this->addChildWithoutEffect(sprite);

	icons.insert(std::pair<int, Sprite*>(key, sprite));
}

void BlurScene::valueSliderChanged(Ref *sender, Control::EventType controlEvent)
{
	if (effectIndex != -1)
	{
		float radius = sliderRadius->getValue();
		float brightness = sliderBrightness->getValue();
		float threshold = sliderThreshold->getValue();

		SpriteAmbientLight *light = (SpriteAmbientLight*)childrenEffect.at(effectIndex);

		auto node = ((ControlSlider*)sender);

		sliderBrightness->setString(StringUtils::format("Brightness(%2.2f)", sliderBrightness->getValue()));
		sliderRadius->setString(StringUtils::format("Radius(%2.2f)", sliderRadius->getValue()));

		switch (node->getTag())
		{
			case SLIDER_RADIUS_TAG:
				light->setRadius(radius);
				break;

			case SLIDER_BRIGHTNESS_TAG:
				light->setBrightness(brightness);
				break;

			default:
				break;
		}
	}

	if (childIndex != -1)
	{
		float radius = sliderRadius->getValue();
		float brightness = sliderBrightness->getValue();
		float threshold = sliderThreshold->getValue();

		auto child = dynamic_cast<SpriteBloom*>(_children.at(childIndex));

		if (child)
		{
			auto node = ((ControlSlider*)sender);

			sliderBrightness->setString(StringUtils::format("Brightness(%2.2f)", sliderBrightness->getValue()));
			sliderRadius->setString(StringUtils::format("Radius(%2.2f)", sliderRadius->getValue()));

			switch (node->getTag())
			{
				case SLIDER_RADIUS_TAG:
					child->setRadius(radius);
					break;

				case SLIDER_BRIGHTNESS_TAG:
					child->setConstract(sliderBrightness->getValue());
					break;

				case SLIDER_THRESHOLD_TAG:
					child->setBrightThreshold(sliderThreshold->getValue());
					break;

				default:
					break;
			}
		}		
	}
}

void BlurScene::colourValueChanged(Ref *sender, extension::Control::EventType controlEvent)
{
	if (effectIndex != -1)
	{
		SpriteAmbientLight *light =(SpriteAmbientLight*) childrenEffect.at(effectIndex);

		light->setColor(Color4F(colorPicker->getColor()));
	}
}

bool BlurScene::onTouchBegan(Touch *touch, Event *e)
{
	effectIndex = -1;
	childIndex = -1;

	for (size_t i = 0; i < childrenEffect.size(); i++)
	{
		auto icon = icons.at(i)->getBoundingBox();

		if (icon.containsPoint(touch->getLocation()))
		{
			effectIndex = i;

			onChildEffectTouched();

			break;
		}
	}

	if (effectIndex != -1)
	{
		return true;
	}

	for (size_t i = 0; i < _children.size(); i++)
	{
		auto node = _children.at(i);

		if (node->getTag() == SPRITE_ALLOW_CONTROL_TAG)
		{
			Rect bound;
			bound.origin = node->getPosition() - Vec2(node->getContentSize().width / 2,
				node->getContentSize().height / 2);

			bound.size = node->getContentSize();

			if (bound.containsPoint(touch->getLocation()))
			{
				childIndex = i;

				onSpriteBloomTouched();

				break;
			}
		}
	}

	if (childIndex == -1 && effectIndex == -1)
	{
		onTouchBackground();
	}

    return true;
}

void BlurScene::onTouchMoved(Touch *touch, Event *e)
{
	/*if (indexTouched != -1)
	{
		nodes.at(indexTouched)->setPosition(nodes.at(indexTouched)->getPosition() + touch->getDelta());
		choosenBox->setPosition(nodes.at(indexTouched)->getPosition());
	}*/

	if (effectIndex != -1)
	{
		auto icon = icons.at(effectIndex);
		icon->setPosition(icon->getPosition() + touch->getDelta());
		childrenEffect.at(effectIndex)->setPosition(icon->getPosition());

		//onChildEffectTouched();
	}

	if (childIndex != -1)
	{
		auto node = _children.at(childIndex);
		node->setPosition(node->getPosition() + touch->getDelta());
	}
}

void BlurScene::onTouchEnded(Touch *touch, Event *e)
{    

}

void BlurScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	isKeyDone = false;
}

void BlurScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (!isKeyDone)
	{
		isKeyDone = true;
		if (keyCode == EventKeyboard::KeyCode::KEY_H)
		{
			for (size_t i = 0; i < icons.size(); i++)
			{
				auto icon = icons.at(i);
				icon->setVisible(!icon->isVisible());
			}
		}
	}
}

void BlurScene::onTouchBackground()
{
	sliderBrightness->setVisible(false);
	sliderRadius->setVisible(false);
	sliderThreshold->setVisible(false);
	colorPicker->setVisible(false);
	colorPicker->setEnabled(false);
}

void BlurScene::onSpriteChanged()
{

}

void BlurScene::onUpdateUniform()
{
	auto p = this->getGLProgram();

	auto lightPos = p->getUniformLocation("lightPosition");
	auto radius = p->getUniformLocation("radius");
	auto color = p->getUniformLocation("lightColor");
	auto brightness = p->getUniformLocation("brightness");
	auto c = light.color;

	glUniform1f(radius, light.radius);
	glUniform1f(brightness, light.brightness);
	glUniform2f(lightPos, light.position.x, light.position.y);
	glUniform4f(color, c.r, c.g, c.b, c.a);
}

void BlurScene::onChildEffectTouched()
{
	if (effectIndex != -1)
	{
		auto node = (SpriteAmbientLight*)childrenEffect.at(effectIndex);
		sliderRadius->setValue(node->getRadius());
		sliderBrightness->setValue(node->getBrightness());
		colorPicker->setColor(Color3B(node->getColor()));

		sliderRadius->setVisible(true);
		sliderBrightness->setVisible(true);
		sliderThreshold->setVisible(false);
		colorPicker->setVisible(true);
		colorPicker->setEnabled(true);

		sliderRadius->getSlider()->setMinimumValue(1.0f);
		sliderRadius->getSlider()->setMaximumValue(2000.0f);
	}
}

void BlurScene::onSpriteBloomTouched()
{
	auto node = dynamic_cast<SpriteBloom*>(_children.at(childIndex));

	if (node)
	{
		sliderRadius->setValue(node->getRadius());
		sliderBrightness->setValue(node->getConstract());
		sliderThreshold->setValue(node->getThreshold());

		sliderRadius->setVisible(true);
		sliderBrightness->setVisible(true);
		sliderThreshold->setVisible(true);

		colorPicker->setVisible(false);

		sliderRadius->getSlider()->setMinimumValue(1.0f);
		sliderRadius->getSlider()->setMaximumValue(64.0f);
	}	
}