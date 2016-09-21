#include "BrightFilter.h"
bool BrightFilter::isInitShader = false;

CustomCommand BrightFilter::command = CustomCommand();

const char* SHADER_BRIGHT_FILTER = "SHADER_BRIGHT_FILTER";



BrightFilter::BrightFilter()
{

}

BrightFilter::~BrightFilter()
{

}

Sprite* BrightFilter::brightFilterSprite(const char* fileName)
{
    initShaders();
    
    Sprite *sprite = Sprite::create(fileName);
    sprite->setGLProgram(ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER));
    sprite->getGLProgram()->use();

    return sprite;
}

void BrightFilter::initShaders()
{
    if (!isInitShader)
    {
        auto brightFilterShader = GLProgram::createWithFilenames("shaders/BrightFilter/brightFilter.vert", "shaders/BrightFilter/brightFilter.frag");

        ShaderCache::getInstance()->addGLProgram(brightFilterShader, SHADER_BRIGHT_FILTER);

        isInitShader = true;
    }
}

void BrightFilter::brightFilter(RenderTexture *texture)
{
    initShaders();
    
    /*texture->begin();

    command.init(texture->getGlobalZOrder());
    command.func = [=]{        

        Tex2F textCoords[6];
        Vec2 vertices[6];
        Color4F colors[6];

        Size size = texture->getSprite()->getContentSize();

        auto currentProgram = ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER);
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

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);

    texture->end();

    Director::getInstance()->getRenderer()->render();*/
    
    ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER)->use();
    ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER)->link();
    ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER)->setUniformsForBuiltins();

    GL::bindTexture2DN(1, Sprite::create("dudvmap.png")->getTexture()->getName());

    Director::getInstance()->getRenderer()->render();
    auto sprite = Sprite::createWithTexture(texture->getSprite()->getTexture());
    sprite->setFlipY(true); 
    sprite->setPosition(sprite->getContentSize().width / 2, sprite->getContentSize().height / 2);
    sprite->setGLProgram(ShaderCache::getInstance()->getGLProgram(SHADER_BRIGHT_FILTER));

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