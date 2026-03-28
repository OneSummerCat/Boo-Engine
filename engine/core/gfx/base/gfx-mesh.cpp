#include "gfx-mesh.h"
#include "../gfx-mgr.h"
#include "../gfx.h"
#include "../gfx-context.h"

GfxMesh::GfxMesh(std::string uuid)
{
    this->_uuid = uuid;
}

void GfxMesh::createMesh(const std::vector<float> &_positions, const std::vector<float> &_normals, const std::vector<float> &_uvs, const std::vector<float> &_uvs1, const std::vector<float> &_uvs2, const std::vector<float> &_colors, const std::vector<float> &_tangents, const std::vector<int> &_indices)
{
    this->_vertices.clear();
    this->_indices.clear();
    
    //准备顶点数据
    int vertexCount = _positions.size() / 3;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", posCount=" << _positions.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", normCount=" << _normals.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", uvCount=" << _uvs.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", uv1Count=" << _uvs1.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", uv2Count=" << _uvs2.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", colorCount=" << _colors.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", tangentCount=" << _tangents.size() << std::endl;
    // std::cout << "createMesh21: idx=" << this->_uuid << ", _indicesCount=" << _indices.size() << std::endl;
    for (int i = 0; i < vertexCount; i++)
    {
        // 顶点坐标
        this->_vertices.push_back(_positions[i * 3]);
        this->_vertices.push_back(_positions[i * 3 + 1]);
        this->_vertices.push_back(_positions[i * 3 + 2]);
        // 法线
        this->_vertices.push_back(_normals[i * 3]);
        this->_vertices.push_back(_normals[i * 3 + 1]);
        this->_vertices.push_back(_normals[i * 3 + 2]);
        // 纹理坐标
        this->_vertices.push_back(_uvs[i * 2]);
        this->_vertices.push_back(_uvs[i * 2 + 1]);
        // 纹理坐标1
        this->_vertices.push_back(_uvs1[i * 2]);
        this->_vertices.push_back(_uvs1[i * 2 + 1]);
        // 纹理坐标2
        this->_vertices.push_back(_uvs2[i * 2]);
        this->_vertices.push_back(_uvs2[i * 2 + 1]);
        // 颜色
        this->_vertices.push_back(_colors[i * 4]);
        this->_vertices.push_back(_colors[i * 4 + 1]);
        this->_vertices.push_back(_colors[i * 4 + 2]);
        this->_vertices.push_back(_colors[i * 4 + 3]);
        // 切线
        this->_vertices.push_back(_tangents[i * 4]);
        this->_vertices.push_back(_tangents[i * 4 + 1]);
        this->_vertices.push_back(_tangents[i * 4 + 2]);
        this->_vertices.push_back(_tangents[i * 4 + 3]);
    }
    // std::cout << "createMesh22: idx=" << this->_uuid << std::endl;
    //准备索引数据
    for (int i = 0; i < _indices.size(); i++)
    {
        this->_indices.push_back(_indices[i]);
    }
    // std::cout << "createMesh4: idx=" << this->_uuid << std::endl;
    this->_createVertexBuffers();
}
void GfxMesh::createUIMesh(const std::vector<float> &_positions, const std::vector<float> &_uvs, const std::vector<int> &_indices)
{
    this->_vertices.clear();
    this->_indices.clear();
    //准备顶点数据
    int vertexCount = _positions.size() / 3;
    for (int i = 0; i < vertexCount; i++)
    {
        // 顶点坐标
        this->_vertices.push_back(_positions[i * 3]);
        this->_vertices.push_back(_positions[i * 3 + 1]);
        this->_vertices.push_back(_positions[i * 3 + 2]);
        // 纹理坐标
        this->_vertices.push_back(_uvs[i * 2]);
        this->_vertices.push_back(_uvs[i * 2 + 1]);
    }
    //准备索引数据
    for (int i = 0; i < _indices.size(); i++)
    {
        this->_indices.push_back(_indices[i]);
    }
    
    this->_createVertexBuffers();
}

void GfxMesh::_createVertexBuffers()
{
    this->_clearVertexBuffers();

    // 顶点缓冲区
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_vertexBuffer,
        &this->_vertexMemory,
        this->_vertices.size() * sizeof(float), // 总字节数
        this->_vertices.data()                  // 数据指针
    );
    // 索引缓冲区（不变）
    GfxMgr::getInstance()->createBuffer(
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &this->_indexBuffer,
        &this->_indexMemory,
        this->_indices.size() * sizeof(uint32_t),
        this->_indices.data());
    this->_indexSize = this->_indices.size();
}
const uint32_t GfxMesh::getIndexCount()
{
    return this->_indices.size();
}



void GfxMesh::_clearVertexBuffers()
{
    if (this->_vertexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::_context->getVkDevice(), this->_vertexBuffer, nullptr);
        this->_vertexBuffer = VK_NULL_HANDLE;
    }
    if (_vertexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::_context->getVkDevice(), this->_vertexMemory, nullptr);
        this->_vertexMemory = VK_NULL_HANDLE;
    }
    if (this->_indexBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(Gfx::_context->getVkDevice(), this->_indexBuffer, nullptr);
        this->_indexBuffer = VK_NULL_HANDLE;
    }
    if (this->_indexMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(Gfx::_context->getVkDevice(), this->_indexMemory, nullptr);
        this->_indexMemory = VK_NULL_HANDLE;
    }
}
bool GfxMesh::equals(const GfxMesh *mesh) const
{
    if (mesh == nullptr)
    {
        return false;
    }
    if (this->_uuid.empty() || mesh->_uuid.empty())
    {
        return false;
    }
    if (this->_uuid != mesh->_uuid)
    {
        return false;
    }
    return true;
}
const std::string & GfxMesh::getUuid()
{
    return this->_uuid;
}
VkBuffer GfxMesh::getVertexBuffer()
{
    return this->_vertexBuffer;
}
VkBuffer GfxMesh::getIndexBuffer()
{
    return this->_indexBuffer;
}
void GfxMesh::destroy()
{
    this->_clearVertexBuffers();
}
// uint32_t GfxMesh::getIndexCount()
// {
//     return this->_indices.size();
// }
// int GfxMesh::getIndexSize()
// {
//     return this->_indexSize;
// }
// void GfxMesh::setInputVertices(std::vector<float> vertices, std::vector<uint32_t> indices)
// {
//     this->_vertices = vertices;
//     this->_indices = indices;
//     this->_createVertexBuffers();
// }