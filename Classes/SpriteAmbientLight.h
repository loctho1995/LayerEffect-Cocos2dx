#pragma once
#include "cocos2d.h"
#include "EffectNode.h"

using namespace cocos2d;

struct AmbientLightParam
{
	cocos2d::Vec2 position;
	cocos2d::Color4F color;
	float radius;
	float brightness;
};

class SpriteAmbientLight : public EffectNode
{
public:
	
	SpriteAmbientLight();
	~SpriteAmbientLight();

	static SpriteAmbientLight* create(Vec2 position, float radius = 10.0f, Color4F color = Color4F::WHITE, float brightness = 1.0f);

	virtual bool init(Vec2 position, float radius = 10.0f, Color4F color = Color4F::WHITE, float brightness = 1.0f);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void setupUniforms();

	void setRadius(float radius);

	void setBrightness(float brightness);

	void setColor(Color4F color);

	float getRadius();

	float getBrightness();

	Color4F getColor();

protected:	
	CustomCommand command;

	AmbientLightParam light;
	GLuint textureID;

	Tex2F textCoords[6];
	Vec2 vertices[6];
	Color4F colors[6];
};

