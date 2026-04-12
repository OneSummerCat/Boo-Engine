#pragma once
// #include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include "../gfx-struct.h"

class GfxBuffer;

struct GfxBufferUBOPool
{
    std::vector<GfxBuffer *> buffers;
};

class GfxBufferUBO
{
private:
    std::map<size_t, GfxBufferUBOPool *> pools;
public:
    GfxBufferUBO();
    GfxBuffer *getBuffer(size_t size);
    void clear();
    ~GfxBufferUBO();
};