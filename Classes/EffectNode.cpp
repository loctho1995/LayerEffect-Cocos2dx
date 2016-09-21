#include "EffectNode.h"

EffectNode::EffectNode()
{

}

EffectNode::~EffectNode()
{

}

bool EffectNode::init()
{
	if (!Node::init())
	{
		return false;
	}

	return true;
}


void EffectNode::setupUniforms()
{

}