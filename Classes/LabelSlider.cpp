#include "LabelSlider.h"

LabelSlider::LabelSlider()
{

}

LabelSlider::~LabelSlider()
{

}

LabelSlider* LabelSlider::create(std::string labelText, float minValue, float maxValue)
{
	LabelSlider* node = new LabelSlider();

	if (node && node->init(labelText, minValue, maxValue))
	{
		node->autorelease();
		
		return node;
	}
	else
	{
		delete node;
	}

	return nullptr;
}

bool LabelSlider::init(std::string labelText, float minValue, float maxValue)
{
	if (!Node::init())
	{
		return false;
	}

	this->setAnchorPoint(Vec2(0, 0));

	label = Label::createWithTTF(labelText, "fonts/tahoma_0.ttf", 16);
	label->setColor(Color3B::RED);
	label->setAnchorPoint(Vec2(0, 0.5));
	label->setPosition(Vec2(0, 0));
	this->addChild(label);

	Size size = label->getContentSize();

	slider = ControlSlider::create("extensions/sliderTrack.png", "extensions/sliderProgress.png", "extensions/sliderThumb.png");
	slider->setAnchorPoint(Vec2(0.0f, 0.5f));
	slider->setPosition(size.width + 60, 0);
	slider->setScale(0.8);
	slider->setMinimumValue(minValue); // Sets the min value of range
	slider->setMaximumValue(maxValue); // Sets the max value of range
	slider->setValue(1.0f);
	
	this->addChild(slider);

	_contentSize = slider->getContentSize() ;
	_contentSize = _contentSize + Size(label->getContentSize().width, 0);
	return true;
}

void LabelSlider::addTargetWithActionForControlEvents(cocos2d::Ref* target, cocos2d::extension::Control::Handler action)
{
	slider->addTargetWithActionForControlEvents(target, action, Control::EventType::VALUE_CHANGED);
}

ControlSlider* LabelSlider::getSlider()
{
	return slider;
}

Label* LabelSlider::getLabel()
{
	return label;
}

float LabelSlider::getValue()
{
	return slider->getValue();
}

void LabelSlider::setValue(float value)
{
	slider->setValue(value);
}

void LabelSlider::setString(std::string text)
{
	label->setString(text);

	//Size size = label->getContentSize();
	//slider->setPosition(size.width + 20, 0);

	_contentSize = slider->getContentSize();
	_contentSize = _contentSize + Size(label->getContentSize().width, 0);
}

void LabelSlider::setMask(int tag)
{
	label->setTag(tag);
	slider->setTag(tag);
}