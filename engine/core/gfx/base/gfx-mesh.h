#pragma once
#include <vulkan/vulkan_core.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstdint>
#include <array>
#include "../gfx-struct.h"

class GfxMesh
{
private:
    std::string _uuid;
    std::vector<float> _vertices;
    std::vector<uint32_t> _indices;

    VkBuffer _vertexBuffer = VK_NULL_HANDLE; /* // 顶点数据Buffer */
    VkBuffer _indexBuffer = VK_NULL_HANDLE;  /* // 顶点index数据buffer */
    VkDeviceMemory _vertexMemory = VK_NULL_HANDLE;
    VkDeviceMemory _indexMemory = VK_NULL_HANDLE;
    int _indexSize;
    void _createVertexBuffers();
    void _clearVertexBuffers();

public:
    // 后续可以加入网格哈希
    GfxMesh(std::string uuid);
    void createMesh(const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices);
    void createUIMesh(const std::vector<float> &_positions, const std::vector<float> &_uvs, const std::vector<int> &_indices);
    VkBuffer getVertexBuffer();
    VkBuffer getIndexBuffer();
    const std::string & getUuid();
    //  uint32_t getVertexCount();
    const uint32_t getIndexCount();
    // int getIndexSize();
    bool equals(const GfxMesh *mesh) const;
    void destroy();
};