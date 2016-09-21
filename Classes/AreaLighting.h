#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class AreaLighting : public Node
{
public:
	
	AreaLighting();
	~AreaLighting();

	static AreaLighting* create(std::vector<Vec2> points, Color4F color = Color4F::WHITE, float brightness = 1.0f);

	virtual bool init(std::vector<Vec2> points, Color4F color = Color4F::WHITE, float brightness = 1.0f);

	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	virtual void onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags);

	void setPoints(std::vector<Vec2> points);

	void setBrightness(float brightness);

	void setColor(Color4F color);

	float getBrightness();

	Color4F getColor();

protected:	
	float brightness;
	Color4F color;
	std::vector<Vec2> points;
	std::vector<Color4F> colors;
	CustomCommand command;
};

