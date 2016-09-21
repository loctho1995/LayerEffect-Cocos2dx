#include "Blur.h"
bool Blur::isInitShader = false;

CustomCommand Blur::command = CustomCommand();

const char* SHADER_HORIZONTAL_BLUR = "SHADER_HORIZONTAL_BLUR";
const char* SHADER_VERTICAL_BLUR = "SHADER_VERTICAL_BLUR";
const char* SHADER_BLUR_KERNEL9 = "SHADER_BLUR_KERNEL9";
const char* SHADER_GAUSSIAN_BLUR = "SHADER_GAUSSIAN_BLUR";

Blur::Blur()
{

}

Blur::~Blur()
{

}

void Blur::initShaders()
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

        isInitShader = true;
    }
}

Sprite* Blur::blurSprite(const char* fileName, int timeBlur)
{
    initShaders();
    
    Sprite *sprite = Sprite::create(fileName);
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    RenderTexture *renderTexture = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);

    renderTexture->begin();
    sprite->visit();
    renderTexture->end();

    Director::getInstance()->getRenderer()->render();

    blur(renderTexture, timeBlur);

    auto sprite2 = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
    sprite2->setFlipY(true);
    return sprite2;
}

void Blur::horizontalBlur(RenderTexture *texture, int radius)
{
    initShaders();

    /*auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    sprite->setFlipY(true);
    auto r = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);

    r->beginWithClear(0, 0, 0, 0);
    sprite->visit();
    r->end();
    Director::getInstance()->getRenderer()->render();

    texture->beginWithClear(0, 0, 0, 0);
    texture->end();

    Director::getInstance()->getRenderer()->render();*/

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

        colors[0] = Color4F::WHITE;
        colors[1] = Color4F::WHITE;
        colors[2] = Color4F::WHITE;
        colors[3] = Color4F::WHITE;
        colors[4] = Color4F::WHITE;
        colors[5] = Color4F::WHITE;

        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

        glUniform2f(glGetUniformLocation(currentProgram->getProgram(), "textureSize"), size.width / radius, size.height / radius);

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();
    Director::getInstance()->getRenderer()->render();
}

void Blur::VerticalBlur(RenderTexture *texture, int radius)
{
    initShaders();

    //CustomCommand command;

    ////Clear texture
    ////deo hieu sao khong Clear thi no khong xoa het sach Frame buffer
    ////tao 1 sprite lay texture tu texutre chinh (copy texture)
    //auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    //sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    //sprite->setFlipY(true);
    //auto r = RenderTexture::create(sprite->getContentSize().width, sprite->getContentSize().height);

    ////copy sang 1 texture render khac (Frame buffer)
    ////de ti laytexture nay render
    //r->beginWithClear(0, 0, 0, 0); 
    //sprite->visit();
    //r->end();
    ////Director::getInstance()->getRenderer()->render();

    ////clear texture hien tai
    //texture->beginWithClear(0, 0, 0, 0);
    //texture->end();
    //Director::getInstance()->getRenderer()->render();

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

void Blur::blurKernel9(RenderTexture *texture, int radius)
{
    initShaders();
    CCLOG("blur");

    /*Size size = texture->getSprite()->getContentSize();

    ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9)->use();
    ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9)->link();
    ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9)->setUniformsForBuiltins();
    glUniform2f(glGetUniformLocation(ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9)->getProgram(), "textureSize"), size.width / radius, size.height / radius);

    Director::getInstance()->getRenderer()->render();

    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setFlipY(true);
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    sprite->setGLProgram(ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9));

    RenderTexture *r = RenderTexture::create(sprite->getContentSize().width + 1.0f, sprite->getContentSize().height + 1.0f);

    r->beginWithClear(0.0, 0.0, 0.0, 0.0);
    sprite->visit();
    r->end();
    Director::getInstance()->getRenderer()->render();

    auto sprite2 = Sprite::createWithTexture(r->getSprite()->getTexture());
    sprite2->setFlipY(true);
    sprite2->setPosition(sprite2->getContentSize().width / 2, sprite2->getContentSize().height / 2);

    texture->beginWithClear(0, 0, 0, 0.0);
    sprite2->visit();
    texture->end();
    Director::getInstance()->getRenderer()->render();*/
    texture->begin();
    command.init(texture->getGlobalZOrder());
    command.func = [=]{

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_BLUR_KERNEL9);
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

void Blur::calculateGaussianWeights(const int points, float* weights)
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

void Blur::gaussianBlur(RenderTexture *texture, Size pixelSize, Vec2 direction, int radius)
{
    initShaders();

    float* weights = new float[64];
    calculateGaussianWeights(radius, weights);


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

void Blur::blur(RenderTexture *texture, int radius)
{
   /* horizontalBlur(texture, timeBlur, 1.0f / 2.0f);
    VerticalBlur(texture, timeBlur, 1.0f / 2.0f);*/

    //blurKernel9(texture, radius);
    horizontalBlur(texture, radius);
    VerticalBlur(texture, radius);
}
