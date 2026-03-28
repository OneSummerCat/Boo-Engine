#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "../base/gfx-render-pass.h"
/**
 * @brief ui 渲染pass
 */
class GfxBuiltinRenderPass : public GfxRenderPass
{

protected:
    void _create() override;
public:
    GfxBuiltinRenderPass(std::string name);
    ~GfxBuiltinRenderPass() override;
};