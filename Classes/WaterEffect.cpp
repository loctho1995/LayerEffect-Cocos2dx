#include "WaterEffect.h"


WaterEffect::WaterEffect()
{
}


WaterEffect::~WaterEffect()
{
}

void WaterEffect::water(RenderTexture *texture)
{
    /*CustomCommand command;

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

        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
    };

    Director::getInstance()->getRenderer()->addCommand(&command);
    texture->end();
    Director::getInstance()->getRenderer()->render();*/
}