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
    float _renderTime = 0.0f;
    GfxMesh *_uiTestMesh = nullptr;
}