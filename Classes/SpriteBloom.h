#pragma once
#include "cocos2d.h"

using namespace cocos2d;

#define MAX_RADIUS 128
class SpriteBloom : public Node
{
public:
	SpriteBloom();
	~SpriteBloom();

	static SpriteBloom* createSpriteBloom(const char* fileName, int radius, float constract = 1.0f, float brightThreshold = 0.0f);
	static SpriteBloom* createSpriteBloom(Sprite* sprite, int radius, float constract = 1.0f, float brightThreshold = 0.0f);

	void setRadius(int radius);

	void setConstract(float constract);

	void setBrightThreshold(float threshold);

	int getRadius();
	float getConstract();
	float getThreshold();

	virtual bool init(const char* fileName, int radius, float constract = 1.0f, float brightThreshold = 0.0f);

	virtual bool init(Sprite* sprite, int radius, float constract = 1.0f, float brightThreshold = 0.0f);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void onBlur(cocos2d::Vec2 direction);

	virtual void onMask();

	virtual void onBrightFilter();

	virtual void onBloom();

protected:
	virtual bool initShaders();
	virtual bool initRenderTexture();
	virtual bool initVertices();

	void calculateGaussianWeights(const int points, float* weights);

	RenderTexture *renderTexture;
	Sprite *originalSprite, *showSprite;
	CustomCommand bloomCommand;

	const char* shaderGaussianBlur;
	const char* shaderBrightFilter;
	const char* shaderBloom;

	float constract, brightThreshold;
	int radius;

	bool isDataChanged;

	Tex2F textCoords[6];
	Vec2 vertices[6];
	Color4F colors[6];
};

