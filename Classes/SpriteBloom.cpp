#include "SpriteBloom.h"
#include "EffectCenter.h"

SpriteBloom::SpriteBloom()
{

}

SpriteBloom::~SpriteBloom()
{

}

SpriteBloom* SpriteBloom::createSpriteBloom(const char* fileName, int radius, float constract, float brightThreshold)
{
	SpriteBloom *sprite = new SpriteBloom();

	if (sprite && sprite->init(fileName, radius, constract, brightThreshold))
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

SpriteBloom* SpriteBloom::createSpriteBloom(Sprite* sprite, int radius, float constract, float brightThreshold)
{
	SpriteBloom *bloom = new SpriteBloom();

	if (bloom && bloom->init(sprite, radius, constract, brightThreshold))
	{
		bloom->autorelease();

		return bloom;
	}
	else
	{
		delete bloom;
	}

	return nullptr;
}

bool SpriteBloom::init(const char* fileName, int radius, float constract, float brightThreshold)
{
	originalSprite = Sprite::create(fileName);
	originalSprite->retain();

	this->radius = radius;
	this->constract = constract;
	this->brightThreshold = brightThreshold;

	initShaders();
	initRenderTexture();
	initVertices();

	return true;
}

bool SpriteBloom::init(Sprite* sprite, int radius, float constract, float brightThreshold)
{
	originalSprite = sprite;
	originalSprite->retain();

	//this->originalSprite = sprite;
	this->radius = radius;
	this->constract = constract;
	this->brightThreshold = brightThreshold;

	initShaders();
	initRenderTexture();
	initVertices();

	return true;
}

bool SpriteBloom::initRenderTexture()
{
	this->Node::setContentSize(originalSprite->getContentSize());
	renderTexture = RenderTexture::create(originalSprite->getContentSize().width, 
												originalSprite->getContentSize().height);

	renderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();

	renderTexture->retain();

	Vec2 oldPos = this->getPosition();
	this->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);

	originalSprite->setPosition(originalSprite->getContentSize().width / 2, 
									originalSprite->getContentSize().height / 2);

	showSprite = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
	this->addChild(showSprite);
	//this->addChild(renderTexture);
	
	return true;
}

bool SpriteBloom::initShaders()
{
	shaderGaussianBlur = "shaderGaussianBlur";
	shaderBrightFilter = "shaderBrighrFilter";
	shaderBloom = "shaderBloom";

	auto gaussianBlur = GLProgram::createWithFilenames("shaders/Blur/gaussianBlur.vert", "shaders/Blur/gaussianBlur.frag");
	auto brightFilterShader = GLProgram::createWithFilenames("shaders/BrightFilter/brightFilter.vert", "shaders/BrightFilter/brightFilter.frag");
	auto bloom = GLProgram::createWithFilenames("shaders/Bloom/bloom.vert", "shaders/Bloom/bloom.frag");

	ShaderCache::getInstance()->addGLProgram(gaussianBlur, shaderGaussianBlur);
	ShaderCache::getInstance()->addGLProgram(bloom, shaderBloom);
	ShaderCache::getInstance()->addGLProgram(brightFilterShader, shaderBrightFilter);

	return true;
}

bool SpriteBloom::initVertices()
{
	Size size = originalSprite->getContentSize();

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

	colors[0] = Color4F::WHITE;
	colors[1] = Color4F::WHITE;
	colors[2] = Color4F::WHITE;
	colors[3] = Color4F::WHITE;
	colors[4] = Color4F::WHITE;
	colors[5] = Color4F::WHITE;

	return true;
}

void SpriteBloom::setRadius(int radius)
{
	if (radius > MAX_RADIUS)
	{
		radius = MAX_RADIUS;
	}

	this->radius = radius;

	isDataChanged = true;
}

void SpriteBloom::setConstract(float constract)
{
	this->constract = constract;

	isDataChanged = true;
}

void SpriteBloom::setBrightThreshold(float threshold)
{
	this->brightThreshold = threshold;

	isDataChanged = true;
}

int SpriteBloom::getRadius()
{
	return radius;
}

float SpriteBloom::getConstract()
{
	return constract;
}

float SpriteBloom::getThreshold()
{
	return brightThreshold;
}

void SpriteBloom::calculateGaussianWeights(const int points, float* weights)
{
	float dx = 1.0f / float(points - 1);
	float sigma = 1.0f / 3.0f;
	float norm = 1.0f / (sqrtf(2.0f * M_PI) * sigma * points);
	float divsigma2 = 0.5f / (sigma * sigma);
	weights[0] = 1.0f;

	for (int i = 1; i < points; i++)
	{
		float x = float(i)*dx;
		weights[i] = norm * expf(-x * x * divsigma2);
		weights[0] -= 2.0f * weights[i];
	}
}

void SpriteBloom::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	if (isDataChanged)
	{
		isDataChanged = false;

		renderTexture->beginWithClear(0, 0, 0, 0);
		bloomCommand.init(this->getGlobalZOrder());
		bloomCommand.func = CC_CALLBACK_0(SpriteBloom::onDraw , this, renderer, transform, flags);
		renderer->addCommand(&bloomCommand);
		renderTexture->end();
	}
}

void SpriteBloom::onDraw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	onBrightFilter();

	onBlur(Vec2(1.0, 0.0));
	onBlur(Vec2(0.0, 1.0));
	onBlur(Vec2(1.0, 1.0));
	onBlur(Vec2(1.0, -1.0));

	onBloom();

	//onMask();
}

void SpriteBloom::onBrightFilter()
{
	auto currentProgram = ShaderCache::getInstance()->getGLProgram(shaderBrightFilter);
	currentProgram->use();
	currentProgram->setUniformsForBuiltins();

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::bindTexture2D(originalSprite->getTexture()->getName());

	glUniform1f(currentProgram->getUniformLocation("threshold"), this->brightThreshold);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}

void SpriteBloom::onBlur(cocos2d::Vec2 direction)
{
	Size size = originalSprite->getContentSize();
	Size pixelSize = Size(1.0f / size.width, 1.0f / size.height);

	auto currentProgram = ShaderCache::getInstance()->getGLProgram(shaderGaussianBlur);
	currentProgram->use();
	currentProgram->setUniformsForBuiltins();

	float* weights = new float[MAX_RADIUS];
	calculateGaussianWeights(radius, weights);

	GLuint pixelSizeLoc = glGetUniformLocation(currentProgram->getProgram(), "pixelSize");
	currentProgram->setUniformLocationWith2f(pixelSizeLoc, pixelSize.width, pixelSize.height);

	GLuint radiusLoc = glGetUniformLocation(currentProgram->getProgram(), "radius");
	currentProgram->setUniformLocationWith1i(radiusLoc, radius);

	GLuint weightsLoc = glGetUniformLocation(currentProgram->getProgram(), "weights");
	currentProgram->setUniformLocationWith1fv(weightsLoc, weights, radius);

	GLuint directionLoc = glGetUniformLocation(currentProgram->getProgram(), "direction");
	currentProgram->setUniformLocationWith2f(directionLoc, direction.x, direction.y);

	//vertices
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::bindTexture2D(renderTexture->getSprite()->getTexture()->getName());

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);
	
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}

void SpriteBloom::onBloom()
{
	auto currentProgram = ShaderCache::getInstance()->getGLProgram(shaderBloom);
	currentProgram->use();
	currentProgram->setUniformsForBuiltins();

	//vertices
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::bindTexture2DN(0, renderTexture->getSprite()->getTexture()->getName());
	GL::bindTexture2DN(1, originalSprite->getTexture()->getName());

	glUniform1f(currentProgram->getUniformLocation("contrast"), this->constract);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}

void SpriteBloom::onMask()
{
	//auto p = ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR);
	//p->use();
	//p->setUniformsForBuiltins();

	//GL::bindTexture2D(originalSprite->getTexture()->getName());
	//GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	//glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

	//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_SRC_ALPHA);
	//glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}