#include "LayerEffect.h"

LayerEffect::LayerEffect()
{

}

LayerEffect::~LayerEffect()
{

}

bool LayerEffect::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

 	texture = RenderTexture::create(this->getContentSize().width, this->getContentSize().height);
	this->addChild(texture);

	Size size = this->getContentSize();

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

void LayerEffect::onUpdateUniform()
{

}

void LayerEffect::setContentSize(const Size & size)
{
	Layer::setContentSize(size);

	if (texture != nullptr)
	{
		texture->removeFromParentAndCleanup(true);

		texture = RenderTexture::create(size.width, size.height);
		texture->getSprite()->setAnchorPoint(Vec2(0.0, 0.0));
		this->addChild(texture);
	}
}

void LayerEffect::addChildEffect(EffectNode *child)
{
	childrenEffect.pushBack(child);
}

void LayerEffect::addChildWithoutEffect(Node *node)
{
	node->setTag(LAYER_EFFECT_CHILD__NONE_EFFECT_TAG);
	this->addChild(node);
}

void LayerEffect::visit(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	texture->beginWithClear(0.0, 0.0, 0.0, 0.0);
	// quick return if not visible. children won't be drawn.
	if (!_visible)
	{
		return;
	}

	// IMPORTANT:
	// To ease the migration to v3.0, we still support the Mat4 stack,
	// but it is deprecated and your code should not rely on it
	_director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	_director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);

	bool visibleByCamera = isVisitableByVisitingCamera();

	int i = 0;

	if (!_children.empty())
	{
		sortAllChildren();
		// draw children zOrder < 0
		for (; i < _children.size(); i++)
		{
			auto node = _children.at(i);

			if (node && node->getLocalZOrder() < 0)
				node->visit(renderer, _modelViewTransform, flags);
			else
				break;
		}
		// self draw
		if (visibleByCamera)
			this->draw(renderer, _modelViewTransform, flags);

		for (auto it = _children.cbegin() + i; it != _children.cend(); ++it)
		{
			if (*it == texture)
				continue;

			(*it)->visit(renderer, _modelViewTransform, flags);
		}
	}

	_director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

	//apply children effect to texture
	childrenCommand.init(this->getGlobalZOrder());
	childrenCommand.func = CC_CALLBACK_0(LayerEffect::onChilrenEffectDraw, this);
	renderer->addCommand(&childrenCommand);
	texture->end();

	texture->visit(renderer, transform, flags);

	for (size_t i = 0; i < _children.size(); i++)
	{
		auto child = _children.at(i);

		if (child->getTag() == LAYER_EFFECT_CHILD__NONE_EFFECT_TAG)
		{
			child->visit(renderer, transform, flags);
		}
	}

	//this->draw(renderer, transform, flags);
}

void LayerEffect::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
	//command.init(this->getGlobalZOrder());
	//command.func = CC_CALLBACK_0(LayerEffect::onDraw, this);
	//renderer->addCommand(&command);
}

void LayerEffect::onChilrenEffectDraw()
{
	Rect rect = this->getBoundingBox();
	rect.size = Size(rect.size.width, rect.size.height);

	vertices[0] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[1] = Vec2(rect.getMinX(), rect.getMaxY());
	vertices[2] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[3] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[4] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[5] = Vec2(rect.getMaxX(), rect.getMinY());

	for (size_t i = 0; i < childrenEffect.size(); i++)
	{
		//apply effect of EffectNode such as lighting,...
		auto node = childrenEffect.at(i);

		node->getGLProgram()->use();
		node->getGLProgram()->setUniformsForBuiltins();
		node->setupUniforms();

		GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
		GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
	}
}

void LayerEffect::onDraw()
{
	CCLOG("fuck");
	Rect rect = this->getBoundingBox();
	vertices[0] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[1] = Vec2(rect.getMinX(), rect.getMaxY());
	vertices[2] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[3] = Vec2(rect.getMinX(), rect.getMinY());
	vertices[4] = Vec2(rect.getMaxX(), rect.getMaxY());
	vertices[5] = Vec2(rect.getMaxX(), rect.getMinY());
	
	//self effect apply
	getGLProgram()->use();
	getGLProgram()->setUniformsForBuiltins();
	onUpdateUniform();

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
	GL::bindTexture2D(texture->getSprite()->getTexture()->getName());

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, 0, textCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)6);
}