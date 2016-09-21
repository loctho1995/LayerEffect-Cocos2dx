#include "EffectCenter.h"

bool EffectCenter::isInitShader = false;

char* SHADER_HORIZONTAL_BLUR = "SHADER_HORIZONTAL_BLUR";
char* SHADER_VERTICAL_BLUR = "SHADER_VERTICAL_BLUR";
char* SHADER_BLUR_KERNEL9 = "SHADER_BLUR_KERNEL9";
char* SHADER_GAUSSIAN_BLUR = "SHADER_GAUSSIAN_BLUR";
char* SHADER_BRIGHT_FILTER = "SHADER_BRIGHT_FILTER";
char* SHADER_BLOOM = "SHADER_BLOOM";
char* SHADER_DISTORTION = "SHADER_DISTORTION";

EffectCenter::EffectCenter()
{
	initShaders();
}

EffectCenter::~EffectCenter()
{

}

void EffectCenter::initShaders()
{
    if (!isInitShader)
    {
        auto pVerticalBlur = GLProgram::createWithFilenames("shaders/Blur/verticalBlur.vert", "shaders/Blur/blur.frag");
        auto pHorizontalBlur = GLProgram::createWithFilenames("shaders/Blur/horizontalBlur.vert", "shaders/Blur/blur.frag");

        auto blurKernel9 = GLProgram::createWithFilenames("shaders/Blur/blurKernel9.vert", "shaders/Blur/blurKernel9.frag");

        auto gaussianBlur = GLProgram::createWithFilenames("shaders/Blur/gaussianBlur.vert", "shaders/Blur/gaussianBlur.frag");

        ShaderCache::getInstance()->addGLProgram(pVerticalBlur, SHADER_VERTICAL_BLUR);
        ShaderCache::getInstance()->addGLProgram(pHorizontalBlur, SHADER_HORIZONTAL_BLUR);
        ShaderCache::getInstance()->addGLProgram(blurKernel9, SHADER_BLUR_KERNEL9);
        ShaderCache::getInstance()->addGLProgram(gaussianBlur, SHADER_GAUSSIAN_BLUR);

        auto brightFilterShader = GLProgram::createWithFilenames("shaders/BrightFilter/brightFilter.vert", "shaders/BrightFilter/brightFilter.frag");
        ShaderCache::getInstance()->addGLProgram(brightFilterShader, SHADER_BRIGHT_FILTER);

        auto bloom = GLProgram::createWithFilenames("shaders/Bloom/bloom.vert", "shaders/Bloom/bloom.frag");
        ShaderCache::getInstance()->addGLProgram(bloom, SHADER_BLOOM);

        auto distortion = GLProgram::createWithFilenames("shaders/distortion/distortion.vert", "shaders/distortion/distortion.frag");
        ShaderCache::getInstance()->addGLProgram(distortion, SHADER_DISTORTION);

        isInitShader = true;
    }
}

void EffectCenter::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);

	CustomCommand *command = new CustomCommand();
	//commands.push_back(command);

	for (size_t i = 0; i < effectDatas.size(); i++)
	{
		CCLOG("fuck");

		EffectData *data = effectDatas.at(i);

		if (data->isClearRenderTexture)
		{
			data->renderTexture->beginWithClear(0, 0, 0, 0);
		}
		else
		{
			data->renderTexture->begin();
		}		

		command->init(data->renderTexture->getGlobalZOrder());
		command->func = CC_CALLBACK_0(EffectCenter::onDraw, this, data);
		renderer->addCommand(command);

		data->renderTexture->end();
	}

	effectDatas.clear();
}

void EffectCenter::onDraw(EffectData *data)
{
	CCLOG("drawn");

	Tex2F textCoords[6];
	Vec2 vertices[6];
	Color4F colors[6];

	Size size = data->originalSprite->getContentSize();

	auto currentProgram = ShaderCache::getInstance()->getGLProgram(data->programName);
	currentProgram->use();
	currentProgram->setUniformsForBuiltins();

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::bindTexture2D(data->originalSprite->getTexture()->getName());

	textCoords[0] = Tex2F(0, 1);
	textCoords[1] = Tex2F(0, 0);
	textCoords[2] = Tex2F(1, 0);
	textCoords[3] = Tex2F(0, 1);
	textCoords[4] = Tex2F(1, 0);
	textCoords[5] = Tex2F(1, 1);

	colors[0] = Color4F::WHITE;
	colors[1] = Color4F::WHITE;
	colors[2] = Color4F::WHITE;
	colors[3] = Color4F::WHITE;
	colors[4] = Color4F::WHITE;
	colors[5] = Color4F::WHITE;

	vertices[0] = Vec2(0, 0);
	vertices[1] = Vec2(0, size.height);
	vertices[2] = Vec2(size.width, size.height);
	vertices[3] = Vec2(0, 0);
	vertices[4] = Vec2(size.width, size.height);
	vertices[5] = Vec2(size.width, 0);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);	

	switch (data->effectType)
	{
		case EffectData::Bloom:
		{
			BloomData *bloomData = ((BloomData*)&data);

			GL::bindTexture2D(bloomData->renderTexture->getSprite()->getTexture()->getName());
			GL::bindTexture2DN(1, bloomData->originalSprite->getTexture()->getName());

			glUniform1f(currentProgram->getUniformLocation("contrast"), bloomData->constract);

			break;
		}

		case EffectData::Blur:
		{			
			BlurData *blurData = (BlurData*)data;
			float radius = blurData->radius;
			Size pixelSize = Size(1.0f / size.width, 1.0f / size.height);

			if (blurData->type == BlurData::BlurTypes::GaussianBlur)
			{
				float* weights = new float[64];
				calculateGaussianWeights(radius, weights);

				GLuint pixelSizeLoc = glGetUniformLocation(currentProgram->getProgram(), "pixelSize");
				currentProgram->setUniformLocationWith2f(pixelSizeLoc, pixelSize.width, pixelSize.height);

				GLuint directionLoc = glGetUniformLocation(currentProgram->getProgram(), "direction");
				currentProgram->setUniformLocationWith2f(directionLoc, blurData->direction.x, blurData->direction.y);

				GLuint radiusLoc = glGetUniformLocation(currentProgram->getProgram(), "radius");
				currentProgram->setUniformLocationWith1i(radiusLoc, radius);

				GLuint weightsLoc = glGetUniformLocation(currentProgram->getProgram(), "weights");
				currentProgram->setUniformLocationWith1fv(weightsLoc, weights, radius);
			}
			else
			{
				glUniform2f(glGetUniformLocation(currentProgram->getProgram(), "textureSize"), size.width / radius,
					size.height / radius);
			}

			break;
		}

		case EffectData::BrightFilter:
		{
			BrightFilterData *brightData = ((BrightFilterData*)&data);

			glUniform1f(currentProgram->getUniformLocation("threshold"), brightData->threshold);

			break;
		}

		default:
			break;
	}

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}

#pragma region - BLUR -

void EffectCenter::horizontalBlur(RenderTexture *texture, int radius)
{
    initShaders();

    CustomCommand command;

    texture->begin();
    command.init(texture->getGlobalZOrder());
    command.func = [=]{

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_HORIZONTAL_BLUR);
        currentProgram->use();
        currentProgram->setUniformsForBuiltins();

        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

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

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

        glUniform2f(glGetUniformLocation(currentProgram->getProgram(), "textureSize"), size.width / radius, size.height / radius);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();

    Director::getInstance()->getRenderer()->render();
}

void EffectCenter::VerticalBlur(RenderTexture *texture, int radius)
{
    initShaders();

    CustomCommand command;

    texture->begin();
    command.init(texture->getGlobalZOrder());
    command.func = [=]{

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getTexture()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_VERTICAL_BLUR);
        currentProgram->use();
        currentProgram->setUniformsForBuiltins();

        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

        textCoords[0] = Tex2F(0, 0);
        textCoords[1] = Tex2F(0, 1);
        textCoords[2] = Tex2F(1, 1);
        textCoords[3] = Tex2F(0, 0);
        textCoords[4] = Tex2F(1, 1);
        textCoords[5] = Tex2F(1, 0);

        //vertices[0] = Vec2(0, size.height);
        //vertices[1] = Vec2(0, 0);
        //vertices[2] = Vec2(size.width, 0);
        //vertices[3] = Vec2(0, size.height);
        //vertices[4] = Vec2(size.width, 0);
        //vertices[5] = Vec2(size.width, size.height);

        vertices[0] = Vec2(0, 0);
        vertices[1] = Vec2(0, size.height);
        vertices[2] = Vec2(size.width, size.height);
        vertices[3] = Vec2(0, 0);
        vertices[4] = Vec2(size.width, size.height);
        vertices[5] = Vec2(size.width, 0);

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

        glUniform2f(glGetUniformLocation(currentProgram->getProgram(), "textureSize"), size.width / radius, size.height / radius);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();

    Director::getInstance()->getRenderer()->render();
}

void EffectCenter::blurKernel9(RenderTexture *texture, int radius)
{
    initShaders();
    
    CustomCommand command;

    texture->beginWithClear(0, 0, 0, 0);
    command.init(texture->getGlobalZOrder());
    command.func = [=]{

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9);
        currentProgram->use();
        currentProgram->setUniformsForBuiltins();

        //GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        textCoords[0] = Tex2F(0, 0);
        textCoords[1] = Tex2F(0, 1);
        textCoords[2] = Tex2F(1, 1);
        textCoords[3] = Tex2F(0, 0);
        textCoords[4] = Tex2F(1, 1);
        textCoords[5] = Tex2F(1, 0);

        vertices[0] = Vec2(0, size.height);
        vertices[1] = Vec2(0, 0);
        vertices[2] = Vec2(size.width, 0);
        vertices[3] = Vec2(0, size.height);
        vertices[4] = Vec2(size.width, 0);
        vertices[5] = Vec2(size.width, size.height);

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

        glUniform2f(glGetUniformLocation(currentProgram->getProgram(), "textureSize"), size.width / radius, size.height / radius);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();
    Director::getInstance()->getRenderer()->render();
}

void EffectCenter::calculateGaussianWeights(const int points, float* weights)
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

void EffectCenter::gaussianBlur(RenderTexture *texture, Size pixelSize, Vec2 direction, int radius)
{
    if (radius > 64)
    {
        radius = 64;
    }

    initShaders();

    float* weights = new float[64];
    calculateGaussianWeights(radius, weights);

    CustomCommand command;

    command;
    texture->begin();
    command.init(texture->getGlobalZOrder());
    command.func = [=]{

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_GAUSSIAN_BLUR);
        currentProgram->use();
        currentProgram->setUniformsForBuiltins();

        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

        GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

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

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

        GLuint pixelSizeLoc = glGetUniformLocation(currentProgram->getProgram(), "pixelSize");
        currentProgram->setUniformLocationWith2f(pixelSizeLoc, pixelSize.width, pixelSize.height);

        GLuint directionLoc = glGetUniformLocation(currentProgram->getProgram(), "direction");
        currentProgram->setUniformLocationWith2f(directionLoc, direction.x, direction.y);

        GLuint radiusLoc = glGetUniformLocation(currentProgram->getProgram(), "radius");
        currentProgram->setUniformLocationWith1i(radiusLoc, radius);

        GLuint weightsLoc = glGetUniformLocation(currentProgram->getProgram(), "weights");
        currentProgram->setUniformLocationWith1fv(weightsLoc, weights, radius);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();

    Director::getInstance()->getRenderer()->render();
}

void EffectCenter::blur(RenderTexture *texture, int radius)
{
    /* horizontalBlur(texture, timeBlur, 1.0f / 2.0f);
    VerticalBlur(texture, timeBlur, 1.0f / 2.0f);*/

    //blurKernel9(texture, radius);
    horizontalBlur(texture, radius);
    VerticalBlur(texture, radius);        
}

void EffectCenter::gaussianBlur8Directions(RenderTexture *texture, Size pixelSize, int radius)
{
    int step = 1;
    int radiusWithStep = radius / step;

    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, 0.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(0.0, 1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, 1.0), radiusWithStep);
    EffectCenter::gaussianBlur(texture, pixelSize, Vec2(1.0, -1.0), radiusWithStep);
}

void EffectCenter::gaussianBlur8Directions(RenderTexture *texture, int radius)
{
    Size pixelSize = Size(1.0f / texture->getSprite()->getContentSize().width, 1.0f / texture->getSprite()->getContentSize().height);

    gaussianBlur8Directions(texture, pixelSize, radius);
}

#pragma endregion

#pragma region - BRIGHT FILTER -
void EffectCenter::brightFilter(RenderTexture *texture, float threshold)
{
    initShaders();
    /*CustomCommand command;
	texture->begin();

    command.init(texture->getGlobalZOrder());
    command.func = [=]{

    Tex2F textCoords[6];
    Vec2 vertices[6];
    Color4F colors[6];

    Size size = texture->getSprite()->getContentSize();

    auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER);
    currentProgram->use();
    currentProgram->setUniformsForBuiltins();
	glUniform1f(currentProgram->getUniformLocation("threshold"), threshold);

    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

    textCoords[0] = Tex2F(0, 0);
    textCoords[1] = Tex2F(0, 1);
    textCoords[2] = Tex2F(1, 1);
    textCoords[3] = Tex2F(0, 0);
    textCoords[4] = Tex2F(1, 1);
    textCoords[5] = Tex2F(1, 0);

    vertices[0] = Vec2(0, size.height);
    vertices[1] = Vec2(0, 0);
    vertices[2] = Vec2(size.width, 0);
    vertices[3] = Vec2(0, size.height);
    vertices[4] = Vec2(size.width, 0);
    vertices[5] = Vec2(size.width, size.height);

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);

    texture->end();

    Director::getInstance()->getRenderer()->render();*/

    auto p = ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER);
    p->use();
    p->link();
    p->setUniformsForBuiltins();    
    glUniform1f(p->getUniformLocation("threshold"), threshold);

    Director::getInstance()->getRenderer()->render();
    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setFlipY(true);
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    sprite->setGLProgram(p);

    RenderTexture *r = RenderTexture::create(sprite->getContentSize().width + 1.0f, sprite->getContentSize().height + 1.0f);

    r->beginWithClear(0, 0, 0, 0);
    sprite->visit();
    r->end();
    Director::getInstance()->getRenderer()->render();

    r->getSprite()->setScale(1.0f);
    r->getSprite()->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

    texture->beginWithClear(0, 0, 0, 0);
    r->getSprite()->visit();
    texture->end();

    Director::getInstance()->getRenderer()->render();
}
#pragma endregion

#pragma region - BLOOM -
void EffectCenter::bloom(RenderTexture *texture, float radius, float brightThreshold, float contrast)
{
    initShaders();

    //create sprite which get texture from RenderTexture
    //then flip by Y
    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setFlipY(true);
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    
    //create new RenderTexture with new Texture then use like highlight texture (original texture) apply to bloom effect
    auto r = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    r->begin();
    sprite->visit();
    r->end();
    Director::getInstance()->getRenderer()->render();

    Size textureSize = texture->getSprite()->getContentSize();
    textureSize = Size(1.0 / textureSize.width, 1.0 / textureSize.height);

    EffectCenter::brightFilter(texture, brightThreshold);
    EffectCenter::gaussianBlur8Directions(texture, textureSize, radius);

    CustomCommand cmd;
    texture->begin();
    cmd.init(texture->getGlobalZOrder());

    cmd.func = [=]{
        Tex2F textCoords[6];
        Vec2 vertices[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_BLOOM);
        currentProgram->use();
        currentProgram->setUniformsForBuiltins();

        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        GL::bindTexture2D(texture->getSprite()->getTexture()->getName());
        GL::bindTexture2DN(1, r->getSprite()->getTexture()->getName());
        
        glUniform1f(currentProgram->getUniformLocation("contrast"), contrast);

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

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&cmd);
    texture->end();
    Director::getInstance()->getRenderer()->render();
}
#pragma endregion

void EffectCenter::distortion(RenderTexture *texture, float weightX, float weightY)
{
    initShaders();

    //create sprite which get texture from RenderTexture
    //then flip by Y
    auto spriteMap = Sprite::create("9557.png");
    //sprite->setFlipY(true);
    spriteMap->setPosition(spriteMap->getContentSize().width / 2, spriteMap->getContentSize().height / 2);

    //create new RenderTexture with new Texture then use like highlight texture (original texture) apply to bloom effect
    auto r2 = RenderTexture::create(spriteMap->getContentSize().width, spriteMap->getContentSize().height);
    r2->begin();
    spriteMap->visit();
    r2->end();

    //Director::getInstance()->getRenderer()->render();
    auto p = ShaderCache::getInstance()->getGLProgram(SHADER_DISTORTION);
    p->use();
    p->link();
    p->setUniformsForBuiltins();

    GL::bindTexture2DN(1, r2->getSprite()->getTexture()->getName());
    glUniform1f(p->getUniformLocation("weightX"), weightX);
    glUniform1f(p->getUniformLocation("weightY"), weightY);

    Director::getInstance()->getRenderer()->render();
    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setFlipY(true);
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    sprite->setGLProgram(p);

    RenderTexture *r = RenderTexture::create(sprite->getContentSize().width + 1.0f, sprite->getContentSize().height + 1.0f);

    r->beginWithClear(0, 0, 0, 0);
    sprite->visit();
    r->end();
    Director::getInstance()->getRenderer()->render();

    r->getSprite()->setScale(1.0f);
    r->getSprite()->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

    texture->beginWithClear(0, 0, 0, 0);
    r->getSprite()->visit();
    texture->end();

    Director::getInstance()->getRenderer()->render();

    ////create sprite which get texture from RenderTexture
    ////then flip by Y
    //auto sprite = Sprite::create("dudvmap1.png");
    ////sprite->setFlipY(true);
    //sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);

    ////create new RenderTexture with new Texture then use like highlight texture (original texture) apply to bloom effect
    //auto r = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);
    //r->begin();
    //sprite->visit();
    //r->end();
    //Director::getInstance()->getRenderer()->render();

    //CustomCommand cmd;
    //texture->begin();
    //cmd.init(texture->getGlobalZOrder());

    //cmd.func = [=]{
    //    Tex2F textCoords[6];
    //    Vec2 vertices[6];

    //    Size size = texture->getSprite()->getContentSize();

    //    auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_DISTORTION);
    //    currentProgram->use();
    //    currentProgram->setUniformsForBuiltins();

    //    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    //    GL::bindTexture2D(texture->getSprite()->getTexture()->getName());
    //    GL::bindTexture2DN(1, r->getSprite()->getTexture()->getName());

    //    glUniform1f(currentProgram->getUniformLocation("weight"), weight);

    //    textCoords[0] = Tex2F(0, 0);
    //    textCoords[1] = Tex2F(0, 1);
    //    textCoords[2] = Tex2F(1, 1);
    //    textCoords[3] = Tex2F(0, 0);
    //    textCoords[4] = Tex2F(1, 1);
    //    textCoords[5] = Tex2F(1, 0);

    //    vertices[0] = Vec2(0, 0);
    //    vertices[1] = Vec2(0, size.height);
    //    vertices[2] = Vec2(size.width, size.height);
    //    vertices[3] = Vec2(0, 0);
    //    vertices[4] = Vec2(size.width, size.height);
    //    vertices[5] = Vec2(size.width, 0);

    //    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    //    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);

    //    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    //};

    //Director::getInstance()->getRenderer()->addCommand(&cmd);
    //texture->end();
    //Director::getInstance()->getRenderer()->render();
}