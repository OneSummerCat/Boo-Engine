#include "alpha.h"
#include "../../boo.h"
#include "../../log.h"
#include "../event/event.h"
#include "../view/view.h"
#include "../scene/node-2d.h"
#include "../scene/node-3d.h"
#include "../renderer/camera.h"
#include "../renderer/ui/ui-sprite.h"
#include "../assets/assets-manager.h"
#include "../gfx/gfx-mgr.h"

namespace Boo
{

  Alpha::Alpha(const std::string name, const std::string uuid)
      : Scene(name, uuid),
        _width(0),
        _height(0),
        _uiCamera(nullptr),
        _ndAlpha(nullptr),
        _spriteAlpha(nullptr),
        _textureBg(nullptr),
        _ndSplash(nullptr),
        _spriteSplash(nullptr),
        _alphaDuration(1.0f),
        _splashAlphaNum(0.0f),
        _isAlphaEnd(false)
  {
    this->_init();
  }

  void Alpha::_init()
  {
    LOGI("[Alpha]: init");
    this->_initCamera();
    this->_initAlpha();
    // 添加text
    // Node2D *node2d = this->getRoot2D();
    // Node2D *ndText = new Node2D("Text");
    // node2d->addChild(ndText);
    // ndText->setPosition(0.0f, 100.0f, 0.0f);
    // UIText *compText = dynamic_cast<UIText *>(ndText->addComponent("UIText"));
    // if (compText != nullptr)
    // {
    //   compText->setText("Alpha Boo!");
    // }
  }
  void Alpha::_initCamera()
  {
    Node2D *node2d = this->getRoot2D();
    Node2D *ndCamera = new Node2D("UI-Camera");
    node2d->addChild(ndCamera);
    ndCamera->setPosition(0.0f, 0.0f, 100.0f);
    this->_uiCamera = dynamic_cast<Camera *>(ndCamera->addComponent("Camera"));
    this->_uiCamera->addGroupID(NodeGroup::Node2D);
    this->_uiCamera->setProjection(CameraProjection::Ortho);
  }
  void Alpha::_initAlpha()
  {
    this->_initAlphaBg();
    this->_initAlphaSplash();
  }
  void Alpha::_initAlphaBg()
  {
    // std::cout << "initAlphaBg" << std::endl;
    Node2D *node2d = this->getRoot2D();
    this->_ndAlpha = new Node2D("Alpha");
    this->_ndAlpha->setSize(view->getDesignWidth(), view->getDesignHeight());
    node2d->addChild(this->_ndAlpha);
    // 添加alpha
    this->_spriteAlpha = dynamic_cast<UISprite *>(this->_ndAlpha->addComponent("UISprite"));
    if (this->_spriteAlpha != nullptr)
    {
      this->_spriteAlpha->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteAlpha->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
  }
  void Alpha::_initAlphaSplash()
  {
    // std::cout << "initAlphaSplash" << std::endl;
    Node2D *node2d = this->getRoot2D();
    // 添加logo splash
    this->_ndSplash = new Node2D("Splash");
    node2d->addChild(this->_ndSplash);
    this->_ndSplash->setPosition(0.0f, 50.0f, 0.0f);
    this->_ndSplash->setSize(144.0f, 144.0f);
    Component *compSplash = this->_ndSplash->addComponent("UISprite");
    if (compSplash != nullptr)
    {
      this->_spriteSplash = dynamic_cast<UISprite *>(compSplash);
      this->_spriteSplash->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteSplash->setColor(1.0f, 1.0f, 1.0f, 0.0f);
      this->_spriteSplash->setTexture(AssetBuiltinTexture::Splash);
    }
  }
  void Alpha::update(float deltaTime)
  {
    Scene::update(deltaTime);
    this->_updateLogoAlpha(deltaTime);
  }
  void Alpha::_updateLogoAlpha(float deltaTime)
  {
    if (this->_spriteSplash == nullptr)
    {
      return;
    }
    if (this->_splashAlphaNum > this->_alphaDuration)
    {
      if (this->_isAlphaEnd == false)
      {
        this->_isAlphaEnd = true;
        event->emit(EventEngine_Launch);
      }
      return;
    }
    this->_splashAlphaNum += deltaTime;
    float alpha = this->_splashAlphaNum / this->_alphaDuration;
    this->_spriteSplash->setAlpha(alpha);
  }

  void Alpha::destroy()
  {
    Scene::destroy();
  }
  Alpha::~Alpha() {}

} // namespace Boo
