#pragma once
#include "asset.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include "../gfx/gfx-mgr.h"

class GfxTexture;
namespace Boo
{
  class TextureAsset : public Asset
  {
  private:
    int _width = 0;
    int _height = 0;
    int _channels = 0;
    std::vector<uint8_t> _pixelsVector;
    GfxTexture *gfxTexture = nullptr;

  protected:
  public:
    TextureAsset();
    TextureAsset(std::string uuid);
    TextureAsset(std::string uuid, std::string path, std::string name);
    /**
     * @brief 创建纹理资产
     * @param width 纹理宽度
     * @param height 纹理高度
     * @param channels 纹理通道数
     * @param pixelsVector 像素数据
     */
    void create(int width, int height, int channels,
                std::vector<uint8_t> pixelsVector, GfxTextureFormat format);
    
    /**
     * @brief 获取纹理宽度
     *
     * @return int
     */
    const int getWidth() const { return _width; }
    /**
     * @brief 获取纹理高度
     *
     * @return int
     */
    const int getHeight() const { return _height; }
    /**
     * @brief 获取纹理通道数
     *
     * @return int
     */
    int channels() const { return _channels; }

    void destroy() override;
    ~TextureAsset() {}
  };

} // namespace Boo
