#include "AreaLighting.h"

AreaLighting::AreaLighting()
{

}
AreaLighting::~AreaLighting()
{

}

AreaLighting* AreaLighting::create(std::vector<Vec2> points, Color4F color, float brightness)
{
	AreaLighting* sprite = new AreaLighting();

	if (sprite && sprite->init(points, color, brightness))
	{
		sprite->autorelease();

		return sprite;
	}
	else
	{
		delete sprite;
	}
	
	return nullptr;
}

bool AreaLighting::init(std::vector<Vec2> points, Color4F color, float brightness)
{
	this->setBrightness(brightness);
	this->setPoints(points);
	this->setColor(color);

	auto p = GLProgram::createWithFilenames("shaders/Lighting/areaLight.vert", 
												"shaders/Lighting/areaLight.frag");
	this->setGLProgram(p);

	return true;
}

void AreaLighting::setPoints(std::vector<Vec2> points)
{
	this->points = points;
}

void AreaLighting::setBrightness(float brightness)
{
	this->brightness = brightness;
}

void AreaLighting::setColor(Color4F color)
{
	this->color = color;

	for (size_t i = 0; i < points.size(); i++)
	{
		colors.push_back(color);
	}
}

float AreaLighting::getBrightness()
{
	return brightness;
}

Color4F AreaLighting::getColor()
{
	return color;
}

void AreaLighting::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	command.init(this->getGlobalZOrder());
	command.func = CC_CALLBACK_0(AreaLighting::onDraw, this, renderer, transform, flags);
	renderer->addCommand(&command);
}

void AreaLighting::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	brightness = 0.5;
	//for (size_t i = 0; i < colors.size(); i++)
	//{
	//	Color4F c = colors.at(i);
	//	colors.at(i) = Color4F(c.r * brightness, c.g * brightness, c.b * brightness, c.a * brightness);
	//}

	Vec2 *vertices = points.data();
	Color4F *cs = colors.data();

	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins();

	auto pos = getGLProgram()->getUniformLocation("brightness");
	getGLProgram()->setUniformLocationWith1f(pos, brightness);

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, cs);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)points.size());
}