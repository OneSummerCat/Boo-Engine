#include "ui-text.h"

#include "../../boo.h"
#include "../../scene/node-2d.h"
#include "../../renderer/camera.h"
#include "../../assets/assets-manager.h"

namespace Boo {

	UIText::UIText(std::string name, Node* node, std::string uuid) : UIRenderer(name, node, uuid),
		_text(""),
		_fontSize(30),
		_lineHeight(35)
	{

	}
	void UIText::Awake()
	{
		UIRenderer::Awake();
		MaterialAsset* mtl = dynamic_cast<MaterialAsset*>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
		this->_setMaterial(mtl);
	}
	void UIText::Enable()
	{
		UIRenderer::Enable();
	}

	void UIText::setText(std::string text)
	{
		if (this->_text == text)
		{
			return;
		}
		// this->_text = text;
		// this->_clearTexture();
		// FreetypeMgr::getInstance()->create(text, this->_fontTexture);
		// this->_setTexture(this->_fontTexture.texture);
		// this->_updateNodeSize();
	}
	void UIText::setSize(int fontSize) {
		this->_fontSize = fontSize;
		this->_updateNodeSize();
	}
	void UIText::setLineHeight(int lineHeight) {
		this->_lineHeight = lineHeight;
	}
	void UIText::setColor(Color& color)
	{
		if (color.getR() == this->_color.getR() &&
			color.getG() == this->_color.getG() &&
			color.getB() == this->_color.getB() &&
			color.getA() == this->_color.getA())
		{
			return;
		}
		this->_setColor(color.getR(), color.getG(), color.getB(), color.getA());
	}
	void UIText::setColor(std::string color)
	{
		if (color == this->_color.hexString())
		{
			return;
		}
		Color c(color);
		this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIText::setColor(float r, float g, float b, float a)
	{
		if (r == this->_color.getR() && g == this->_color.getG() &&
			b == this->_color.getB() && a == this->_color.getA())
		{
			return;
		}
		this->_setColor(r, g, b, a);
	}
	void UIText::setAlpha(float alpha)
	{
		if (alpha == this->_color.getA())
			return;
		Color c(this->_color);
		c.setA(alpha);
		this->_setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIText::Update(float deltaTime) {
		UIRenderer::Update(deltaTime);
	}
	void UIText::LateUpdate(float deltaTime) {
		UIRenderer::LateUpdate(deltaTime);
	}
	void UIText::Render(Camera* camera) {
		if (this->_text.size() <= 0) {
			return;
		}
		UIRenderer::Render(camera);
	}
	void UIText::_updateNodeSize()
	{
		Node2D* node2d = dynamic_cast<Node2D*>(this->_node);
		if (node2d == nullptr)
		{
			return;
		}

		TextureAsset* texture = this->_fontTexture.texture;
		if (texture == nullptr) {
			return;
		}
		float width= texture->getWidth()*(this->_fontSize/50.0f);
	    float height= texture->getHeight() * (this->_fontSize / 50.0f);
		node2d->setSize(width, height);
	}
	void UIText::_clearTexture()
	{
		if (this->_fontTexture.texture != nullptr)
		{
			this->_fontTexture.texture->destroy();
			this->_fontTexture.texture = nullptr;
		}
	}
	void UIText::Disable()
	{
		UIRenderer::Disable();
	}

	void UIText::destroy()
	{
		UIRenderer::destroy();
	}
	UIText::~UIText() {}

} // namespace Boo
