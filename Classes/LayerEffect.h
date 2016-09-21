#pragma once
#include "cocos2d.h"
#include "EffectNode.h"

using namespace cocos2d;

#define LAYER_EFFECT_CHILD__NONE_EFFECT_TAG 8296

class LayerEffect : public Layer
{
public:
	LayerEffect();
	~LayerEffect();
	
	virtual void addChildEffect(EffectNode *child);

	virtual void addChildWithoutEffect(Node *node);

	virtual bool init();

	virtual void visit(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);

	virtual void onDraw();

	virtual void onChilrenEffectDraw();

	virtual void setContentSize(const Size & size);

	//override function for setup uniform
	virtual void onUpdateUniform();

protected:
	RenderTexture *texture;
	CustomCommand command, childrenCommand;

	Vector<EffectNode*> childrenEffect;

	Tex2F textCoords[6];
	Vec2 vertices[6];
	Color4F colors[6];
};

