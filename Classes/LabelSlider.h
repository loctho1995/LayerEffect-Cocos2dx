#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::ui;

class LabelSlider : public Node
{
public:
	static LabelSlider* create(std::string labelText, float minValue, float maxValue);
		;
	LabelSlider();
	~LabelSlider();

	virtual bool init(std::string labelText, float minValue, float maxValue);

	virtual void addTargetWithActionForControlEvents(Ref* target, cocos2d::extension::Control::Handler action);

	virtual ControlSlider* getSlider();

	virtual Label* getLabel();

	virtual void setString(std::string text);

	virtual float getValue();

	virtual void setValue(float value);

	//like setTag 
	virtual void setMask(int tag);

protected:
	cocos2d::Label *label;
	ControlSlider *slider;
};

