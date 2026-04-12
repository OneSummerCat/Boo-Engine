#include "gfx.h"
#include "gfx-context.h"
#include "gfx-renderer.h"
#include "base/gfx-shader.h"
#include "base/gfx-texture.h"
#include "base/gfx-mesh.h"
#include "base/gfx-buffer-ubo.h"
#include "base/gfx-buffer-instance.h"
#include "base/gfx-render-texture.h"


namespace Gfx
{
    int _viewWidth = 0;
    int _viewHeight = 0;

    GfxContext *_context = nullptr;
    GfxRenderer *_renderer = nullptr;
    float _time = 0.0f;
    std::map<std::string, GfxShader *> _shaders;
    std::map<std::string, GfxTexture *> _textures;
    std::map<std::string, GfxMesh *> _meshes;
    GfxBufferUBO *_bufferUBO = nullptr;
    GfxBufferInstance *_bufferInstance = nullptr;
    int _drawCount = 0;
    /**
     * @brief 完整帧耗时（含fence等待）毫秒
     */
    float _frameTime = 0.0f;
    /**
     * @brief 纯CPU提交耗时（不含fence等待）毫秒
     */ 
    float _submitTime = 0.0f;
    std::array<float, 4> _mainLitDir = {0.0f, 0.0f, 0.0f, 0.0f};
    std::array<float, 4> _mainLitColor = {0.0f, 0.0f, 0.0f, 0.0f};
}