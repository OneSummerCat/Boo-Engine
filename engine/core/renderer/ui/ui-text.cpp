#include "ui-text.h"

#include "../../../boo.h"
#include "../../scene/node-2d.h"
#include "../../renderer/camera.h"
#include "../../assets/assets-manager.h"
#include "../../gfx/base/gfx-mesh.h"
#include "../../gfx/base/gfx-material.h"

namespace Boo
{

	UIText::UIText(std::string name, Node *node, std::string uuid) : UIRenderer(name, node, uuid),
																	 _text(""),
																	 _fontSize(30),
																	 _lineHeight(35),
																	 _meshAsset(nullptr)
	{
		this->_createDefaultMaterial();
		this->_createDefaultMesh();
		this->_fontRealSize = 50;
	}
	void UIText::_createDefaultMaterial()
	{
		this->_materialAsset = new MaterialAsset(AssetBuiltinMaterial::UI, "", "");
		MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
		this->_materialAsset->create(mtl);
	}

	void UIText::_createDefaultMesh()
	{
		this->_meshAsset = new MeshAsset();
		MeshPrimitive primitive;
		primitive.index = 0;
		primitive.type = 0;
		std::vector<float> positions = {};
		std::vector<float> uvs = {};
		std::vector<uint32_t> indices = {};
		uint32_t MAX_SIZE = 100; // 最大字符数
		for (int i = 0; i < MAX_SIZE; i++)
		{
			// 每个字符两个三角形,6个顶点
			// 第一个三角形
			// 顶点0 左上角
			positions.push_back(-0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
			// 顶点1 左下角
			positions.push_back(-0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(1.0f);
			// 顶点2 右下角
			positions.push_back(0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(1.0f);
			// 第二个三角形
			// 顶点3 左上角
			positions.push_back(-0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(0.0f);
			uvs.push_back(0.0f);
			// 顶点4 右下角
			positions.push_back(0.5f);
			positions.push_back(-0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(1.0f);
			// 顶点5 右上角
			positions.push_back(0.5f);
			positions.push_back(0.5f);
			positions.push_back(0.0f);
			uvs.push_back(1.0f);
			uvs.push_back(0.0f);
		}
		uint32_t vertexCount = positions.size() / 3;
		for (int i = 0; i < vertexCount; i++)
		{
			indices.push_back(i * 3);
			indices.push_back(i * 3 + 1);
			indices.push_back(i * 3 + 2);
		}
		primitive._positions = positions;
		primitive._uvs = uvs;
		primitive._indices = indices;
		this->_meshAsset->createDynamic({primitive});
	}

	void UIText::Awake()
	{
		UIRenderer::Awake();
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
		std::cout << "setText:" << text << std::endl;
		this->_text = text;
		TextLayoutResult result = FreetypeMgr::getInstance()->create(text, 50);
		this->_realWidth = result.width;
		this->_realHeight = result.height;
		MeshPrimitive primitive;
		primitive._positions = result.batches[0].positions;
		primitive._uvs = result.batches[0].uvs;
		primitive._indices = result.batches[0].indices;
		this->_meshAsset->updateDynamicSubMesh(0, primitive);
		this->_materialAsset->setTexture(result.batches[0].texture);
		this->_updateNodeSize();
	}
	void UIText::setSize(int fontSize)
	{
		this->_fontSize = fontSize;
		this->_updateNodeSize();
	}
	void UIText::setLineHeight(int lineHeight)
	{
		this->_lineHeight = lineHeight;
	}
	void UIText::_updateNodeSize()
	{
		Node2D *node2d = dynamic_cast<Node2D *>(this->_node);
		if (node2d == nullptr)
		{
			return;
		}
		float rate = this->_realHeight / (float)this->_fontRealSize;
		node2d->setSize(this->_realWidth * rate, this->_realHeight * rate);
	}
	void UIText::setColor(Color &color)
	{
		if (color.getR() == this->_color.getR() &&
			color.getG() == this->_color.getG() &&
			color.getB() == this->_color.getB() &&
			color.getA() == this->_color.getA())
		{
			return;
		}
		this->setColor(color.getR(), color.getG(), color.getB(), color.getA());
	}
	void UIText::setColor(std::string color)
	{
		if (color == this->_color.hexString())
		{
			return;
		}
		Color c(color);
		this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIText::setAlpha(float alpha)
	{
		if (alpha == this->_color.getA())
			return;
		Color c(this->_color);
		c.setA(alpha);
		this->setColor(c.getR(), c.getG(), c.getB(), c.getA());
	}
	void UIText::setColor(float r, float g, float b, float a)
	{
		if (r == this->_color.getR() && g == this->_color.getG() &&
			b == this->_color.getB() && a == this->_color.getA())
		{
			return;
		}
		this->_color = Color(r, g, b, a);
		this->_color.set(r, g, b, a);
		this->_materialAsset->setUIColor(r, g, b, a);
	}
	void UIText::setMaterial(std::string material)
	{
		MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(material, true));
		if (mtl == nullptr)
		{
			LOGW("[UIText]:setMaterial: material %s not found", material.c_str());
			return;
		}
		this->setMaterial(mtl);
	}
	void UIText::setMaterial(MaterialAsset *material)
	{
		if (material == nullptr)
		{
			LOGW("[UIText]:setMaterial: material %s not found", material->getName().c_str());
			return;
		}
		this->_materialAsset->create(material);
	}

	void UIText::Update(float deltaTime)
	{
		UIRenderer::Update(deltaTime);
	}
	void UIText::LateUpdate(float deltaTime)
	{
		UIRenderer::LateUpdate(deltaTime);
	}
	void UIText::Render(Camera *camera)
	{
		if (!this->_canRenderer(camera))
		{
			return;
		}
		if (this->_text.size() <= 0)
		{
			return;
		}
		UIRenderer::Render(camera);
		// 1. 先添加模型矩阵 (16个float)
		const Mat4 &matrix = this->_node2D->getUIWorldMatrix();
		Mat4::multiply(matrix, view->getFitMatrix(), this->_uiViewMatrix);
		// 更新世界矩阵
		this->_materialAsset->setModelWorldMatrix(this->_uiViewMatrix.data());
		// std::cout << "Render111: " << this->_name << std::endl;
		GfxMgr::getInstance()->submitRenderObject(camera->getUuid(), this->_materialAsset->getGfxMaterial(), this->_meshAsset->getGfxMesh(0));
		// 增加渲染物体数量
		profiler->addObjectCount(1);
	}

	void UIText::Disable()
	{
		UIRenderer::Disable();
	}

	void UIText::destroy()
	{
		UIRenderer::destroy();
		delete this->_materialAsset;
		delete this->_meshAsset;
		this->_meshAsset = nullptr;
		this->_materialAsset = nullptr;
	}
	UIText::~UIText() {}

} // namespace Boo
