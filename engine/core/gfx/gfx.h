#pragma once
#include <map>
#include <string>
#include <vulkan/vulkan_core.h>

class GfxContext;
class GfxRenderer;
class GfxShader;
class GfxTexture;
class GfxBufferUBO;
class GfxBufferInstance;
class GfxRenderTexture;
class GfxMesh;

namespace Gfx
{
	extern int _viewWidth;
	extern int _viewHeight;
	extern GfxContext *_context;
	extern GfxRenderer *_renderer;
	extern float _time;
	extern std::map<std::string, GfxShader *> _shaders;
	extern std::map<std::string, GfxTexture *> _textures;
	extern std::map<std::string, GfxMesh *> _meshes;
	extern GfxBufferUBO *_bufferUBO;
	extern GfxBufferInstance *_bufferInstance;
	extern int _drawCount;
	extern float _renderTime;
	
	extern GfxMesh *_uiTestMesh;
}