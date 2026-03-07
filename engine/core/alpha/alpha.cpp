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
        _ndLogo(nullptr),
        _spriteLogo(nullptr),
        _textureLogo(nullptr),
        _logoTxWidth(0.0f),
        _logoTxHeight(0.0f),
        _alphaDuration(1.0f),
        _logoAlphaNum(0.0f),
        _isAlphaEnd(false)
  {
    this->_init();
  }

  void Alpha::_init()
  {
    LOGI("[Alpha]: init");
    this->_initCamera();
    this->_initAlpha();
  }
  void Alpha::_initCamera()
  {
    Node2D *node2d = this->getRoot2D();
    Node2D *ndCamera = new Node2D("UI-Camera");
    node2d->addChild(ndCamera);
    ndCamera->setPosition(0.0f, 0.0f, -100.0f);
    this->_uiCamera = dynamic_cast<Camera *>(ndCamera->addComponent("Camera"));
  }
  void Alpha::_initAlpha()
  {
    this->_initAlphaBg();
    this->_initAlphaLogo();
  }
  void Alpha::_initAlphaBg()
  {
    Node2D *node2d = this->getRoot2D();
    this->_ndAlpha = new Node2D("Alpha");
    this->_ndAlpha->setSize(view->getDesignWidth(), view->getDesignHeight());
    node2d->addChild(this->_ndAlpha);
    // 添加alpha
    this->_spriteAlpha = dynamic_cast<UISprite *>(this->_ndAlpha->addComponent("UISprite"));
    LOGI("[Alpha]: _initAlpha, %s", this->_ndAlpha->getName().c_str());
    if (this->_spriteAlpha != nullptr)
    {
      this->_spriteAlpha->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteAlpha->setTexture(AssetBuiltinTexture::Default);
      this->_spriteAlpha->setColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
  }
  void Alpha::_initAlphaLogo()
  {
    Node2D *node2d = this->getRoot2D();
    // 添加logo
    this->_ndLogo = new Node2D("Logo");
    node2d->addChild(this->_ndLogo);
    this->_ndLogo->setPosition(0.0f, 100.0f, 0.0f);
    Component *compLogo = this->_ndLogo->addComponent("UISprite");
    if (compLogo != nullptr)
    {
      this->_spriteLogo = dynamic_cast<UISprite *>(compLogo);
      this->_spriteLogo->setMaterial(AssetBuiltinMaterial::UI);
      this->_spriteLogo->setTexture(AssetBuiltinTexture::Logo);
      this->_spriteLogo->setEnabled(true);
      this->_spriteLogo->setColor(1.0f, 1.0f, 1.0f, 0.0f);
    }

    Asset *text = assetsManager->getAsset("builtin::logo.png");
    TextureAsset *texture = dynamic_cast<TextureAsset *>(text);
    this->_logoTxWidth = texture->width();
    this->_logoTxHeight = texture->height();
    float _width = 0.0f;
    float _height = 0.0f;
    if (view->getDesignWidth() > view->getDesignHeight())
    {
      _height = view->getDesignHeight() * 0.5;
      _width = this->_logoTxWidth * (_height / this->_logoTxHeight);
    }
    else
    {
      _width = view->getDesignWidth() * 0.5;
      _height = this->_logoTxHeight * (_width / this->_logoTxWidth);
    }
    this->_ndLogo->setSize(_width, _height);
    // std::cout << "this->_ndLogo name: " << this->_ndLogo->getName() << std::endl;
    // std::cout << "this->_ndLogo width: " << this->_logoTxWidth << std::endl;
    // std::cout << "this->_ndLogo height: " << this->_logoTxHeight << std::endl;
    // std::cout << "view width: " << view->getWidth() << std::endl;
    // std::cout << "view height: " << view->getHeight() << std::endl;
    // std::cout << "design width: " << view->getDesignWidth() << std::endl;
    // std::cout << "design height: " << view->getDesignHeight() << std::endl;
    // std::cout << "logo width: " << _width << std::endl;
    // std::cout << "logo height: " << _height << std::endl;
  }
  void Alpha::update(float deltaTime)
  {
    Scene::update(deltaTime);
    this->_updateLogoAlpha(deltaTime);
  }
  void Alpha::_updateLogoAlpha(float deltaTime)
  {
    if (this->_spriteLogo == nullptr)
    {
      return;
    }
    if (this->_logoAlphaNum > this->_alphaDuration)
    {
      if (this->_isAlphaEnd == false)
      {
        this->_isAlphaEnd = true;
        event->emit(EventEngine_Launch);
      }
      return;
    }
    this->_logoAlphaNum += deltaTime;
    float alpha = this->_logoAlphaNum / this->_alphaDuration;
    this->_spriteLogo->setAlpha(alpha);
  }

  void Alpha::destroy()
  {
    Scene::destroy();
  }
  Alpha::~Alpha() {}

} // namespace Boo
