#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "../gfx-struct.h"
#include "../base/gfx-render-pass.h"

class GfxDefaultRenderPass : public GfxRenderPass
{
private:
protected:
	virtual void _create();

public:
	GfxDefaultRenderPass(std::string name);
	~GfxDefaultRenderPass() override;
};
