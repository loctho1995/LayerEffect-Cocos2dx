#include "Bloom.h"

bool Bloom::isInitShader = false;

const char* SHADER_BLOOM = "SHADER_BLOOM";

Bloom::Bloom()
{

}

Bloom::~Bloom()
{

}

void Bloom::initShaders()
{
    if (!isInitShader)
    {
        auto bloom = GLProgram::createWithFilenames("shaders/Bloom/bloom.vert", "shaders/Bloom/bloom.frag");

        ShaderCache::getInstance()->addGLProgram(bloom, SHADER_BLOOM);

        isInitShader = true;
    }
}

void Bloom::bloom(RenderTexture *texture, Texture2D *highLight)
{
    initShaders();

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
        GL::bindTexture2DN(1, highLight->getName());
        //GL::bindTexture2D(highLight->getName());

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE0, texture->getSprite()->getTexture()->getName());

        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE1, texture->getSprite()->getTexture()->getName());

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