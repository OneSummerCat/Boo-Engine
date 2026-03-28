#include "texture-asset.h"
#include "../../libs/stb/stb_image.h"
#include "../gfx/base/gfx-texture.h"

namespace Boo
{
  TextureAsset::TextureAsset() : Asset(),
                                 _width(0),
                                 _height(0),
                                 _channels(0)
  {
    this->_type = AssetType::Texture;
  }
  TextureAsset::TextureAsset(std::string uuid) : Asset(uuid),
                                                 _width(0),
                                                 _height(0),
                                                 _channels(0)
  {
    this->_type = AssetType::Texture;
  }
  TextureAsset::TextureAsset(std::string uuid, std::string path, std::string name) : Asset(uuid, path, name),
                                                                                     _width(0),
                                                                                     _height(0),
                                                                                     _channels(0)
  {
    this->_type = AssetType::Texture;
  }
  void TextureAsset::create(int width, int height, int channels,
                            std::vector<uint8_t> pixelsVector, GfxTextureFormat format)
  {
    this->_width = width;
    this->_height = height;
    this->_channels = channels;
    this->_pixelsVector = pixelsVector;
    this->gfxTexture = GfxMgr::getInstance()->createTexture(this->_uuid, this->_width, this->_height,
                                                            this->_channels, &this->_pixelsVector, format);
  }

  void TextureAsset::destroy()
  {
    GfxMgr::getInstance()->destroyTexture(this->gfxTexture);
    this->gfxTexture = nullptr;
  }

} // namespace Boo