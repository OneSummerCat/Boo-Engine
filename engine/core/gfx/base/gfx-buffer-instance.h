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

struct GfxBufferInstancePool
{
    std::vector<GfxBuffer *> buffers;
};


class GfxBufferInstance
{
private:
    std::map<size_t, GfxBufferInstancePool *> pools;

public:
    GfxBufferInstance();
     GfxBuffer *getBuffer(size_t size);
     void clear();
    ~GfxBufferInstance();
};