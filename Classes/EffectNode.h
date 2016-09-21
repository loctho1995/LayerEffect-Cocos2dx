#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class EffectNode : public Node
{
public:
	EffectNode();
	~EffectNode();

	virtual bool init();

	virtual void setupUniforms();
};

