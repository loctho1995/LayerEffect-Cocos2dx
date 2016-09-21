#include "SpriteAmbientLight.h"

SpriteAmbientLight::SpriteAmbientLight()
{

}

SpriteAmbientLight::~SpriteAmbientLight()
{

}

SpriteAmbientLight* SpriteAmbientLight::create(Vec2 position, float radius, Color4F color, float brightness)
{
	SpriteAmbientLight* sprite = new SpriteAmbientLight();

	if (sprite && sprite->init(position, radius, color, brightness))
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

bool SpriteAmbientLight::init(Vec2 position, float radius, Color4F color, float brightness)
{
	if (!EffectNode::init())
	{
		return false;
	}

	this->setPosition(position);
	this->setBrightness(brightness);
	this->setRadius(radius);
	this->setAnchorPoint(Vec2(0.5, 0.5));
	this->setColor(color);

	/*auto p = GLProgram::createWithFilenames("shaders/Lighting/ambientLight.vert", 
												"shaders/Lighting/ambientLight.frag");*/

	auto p = GLProgram::createWithFilenames("shaders/ambientLight.vert", "shaders/ambientLight.frag");
	this->setGLProgram(p);

	Size size = this->_contentSize;

	textCoords[0] = Tex2F(0, 0);
	textCoords[1] = Tex2F(0, 1);
	textCoords[2] = Tex2F(1, 1);
	textCoords[3] = Tex2F(0, 0);
	textCoords[4] = Tex2F(1, 1);
	textCoords[5] = Tex2F(1, 0);

	vertices[0] = Vec2(0, 0);
	vertices[1] = Vec2(0, size.height);
	vertices[2] = Vec2(size.width, size.height);
	vertices[3] = Vec2(0, 0);
	vertices[4] = Vec2(size.width, size.height);
	vertices[5] = Vec2(size.width, 0);

	return true;
}

void SpriteAmbientLight::setupUniforms()
{
	auto p = this->getGLProgram();
	auto pos = p->getUniformLocation("lightPosition");
	auto r = p->getUniformLocation("radius");
	auto c = p->getUniformLocation("lightColor");
	auto b = p->getUniformLocation("brightness");

	glUniform1f(r, light.radius);
	glUniform1f(b, light.brightness);
	glUniform2f(pos, this->getPosition().x, this->getPosition().y);
	glUniform4f(c, light.color.r, light.color.g, light.color.b, light.color.a);
}

void SpriteAmbientLight::setRadius(float radius)
{
	light.radius = radius;
	this->_contentSize = Size(radius, radius);
	Size size = this->_contentSize;

	vertices[0] = Vec2(0, 0);
	vertices[1] = Vec2(0, size.height);
	vertices[2] = Vec2(size.width, size.height);
	vertices[3] = Vec2(0, 0);
	vertices[4] = Vec2(size.width, size.height);
	vertices[5] = Vec2(size.width, 0);
}

void SpriteAmbientLight::setBrightness(float brightness)
{
	light.brightness = brightness;
}

void SpriteAmbientLight::setColor(Color4F color)
{
	light.color = color;

	colors[0] = color;
	colors[1] = color;
	colors[2] = color;
	colors[3] = color;
	colors[4] = color;
	colors[5] = color;

	/*colors[0] = Color4F(Color3B(color), 0.1);
	colors[1] = Color4F(Color3B(color), 0.1);
	colors[2] = Color4F(Color3B(color), 0.1);
	colors[3] = Color4F(Color3B(color), 0.1);
	colors[4] = Color4F(Color3B(color), 0.1);
	colors[5] = Color4F(Color3B(color), 0.1);*/
}

float SpriteAmbientLight::getRadius()
{
	return light.radius;
}

float SpriteAmbientLight::getBrightness()
{
	return light.brightness;
}

Color4F SpriteAmbientLight::getColor()
{
	return light.color;
}

void SpriteAmbientLight::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	//command.init(this->getGlobalZOrder());
	//command.func = CC_CALLBACK_0(SpriteAmbientLight::onDraw, this, renderer, transform, flags);
	//renderer->addCommand(&command);
}

void SpriteAmbientLight::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	/*Rect rect = this->getBoundingBox();

	vertices[0] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[1] = Vec2(rect.getMinX(), rect.getMaxY());
	vertices[2] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[3] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[4] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[5] = Vec2(rect.getMaxX(), rect.getMinY());

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

	GL::bindTexture2D(textureID);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);*/
}