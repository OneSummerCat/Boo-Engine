#pragma once
#include <string>
#include <unordered_map>
#include <filesystem>
#include "asset.h"

enum class AssetType
{
    None,
    Image,
    Audio,
    Font,
    Shader,
    Material,
};
// struct AssetPath
// {
//     /**
//      * @brief 资产路径
//      * @param path 绝对路径
//      */
//     std::string path;
//     /**
//      * @brief 资产类型
//      */
//     AssetType type;
// };
// 基础图片资源
const std::string ImageBase = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAB4AAAAeCAIAAAC0Ujn1AAAACXBIWXMAAAsTAAALEwEAmpwYAAAF/mlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPD94cGFja2V0IGJlZ2luPSLvu78iIGlkPSJXNU0wTXBDZWhpSHpyZVN6TlRjemtjOWQiPz4gPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iQWRvYmUgWE1QIENvcmUgNS42LWMxNDIgNzkuMTYwOTI0LCAyMDE3LzA3LzEzLTAxOjA2OjM5ICAgICAgICAiPiA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPiA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIiB4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIgeG1sbnM6cGhvdG9zaG9wPSJodHRwOi8vbnMuYWRvYmUuY29tL3Bob3Rvc2hvcC8xLjAvIiB4bWxuczp4bXBNTT0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL21tLyIgeG1sbnM6c3RFdnQ9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9zVHlwZS9SZXNvdXJjZUV2ZW50IyIgeG1sbnM6dGlmZj0iaHR0cDovL25zLmFkb2JlLmNvbS90aWZmLzEuMC8iIHhtcDpDcmVhdGVEYXRlPSIyMDI1LTA4LTIwVDIyOjAwOjUyKzA4OjAwIiB4bXA6TW9kaWZ5RGF0ZT0iMjAyNS0wOS0xNFQyMTo1Mzo1OCswODowMCIgeG1wOk1ldGFkYXRhRGF0ZT0iMjAyNS0wOS0xNFQyMTo1Mzo1OCswODowMCIgZGM6Zm9ybWF0PSJpbWFnZS9wbmciIHBob3Rvc2hvcDpDb2xvck1vZGU9IjMiIHBob3Rvc2hvcDpJQ0NQcm9maWxlPSJzUkdCIElFQzYxOTY2LTIuMSIgeG1wTU06SW5zdGFuY2VJRD0ieG1wLmlpZDo2MjU2NDJkZC1iMGU4LWM3NGItOWIzNy1mMWYyMDg1OTlhNDAiIHhtcE1NOkRvY3VtZW50SUQ9InhtcC5kaWQ6MzU4OGE1MDgtZjhiOS02ODQ4LTk0N2MtNjg3ZmNmYjFjN2Y0IiB4bXBNTTpPcmlnaW5hbERvY3VtZW50SUQ9InhtcC5kaWQ6MzU4OGE1MDgtZjhiOS02ODQ4LTk0N2MtNjg3ZmNmYjFjN2Y0IiB0aWZmOk9yaWVudGF0aW9uPSIxIj4gPHhtcE1NOkhpc3Rvcnk+IDxyZGY6U2VxPiA8cmRmOmxpIHN0RXZ0OmFjdGlvbj0ic2F2ZWQiIHN0RXZ0Omluc3RhbmNlSUQ9InhtcC5paWQ6MzU4OGE1MDgtZjhiOS02ODQ4LTk0N2MtNjg3ZmNmYjFjN2Y0IiBzdEV2dDp3aGVuPSIyMDI1LTA5LTA3VDIyOjUwOjUzKzA4OjAwIiBzdEV2dDpzb2Z0d2FyZUFnZW50PSJBZG9iZSBQaG90b3Nob3AgQ0MgKFdpbmRvd3MpIiBzdEV2dDpjaGFuZ2VkPSIvIi8+IDxyZGY6bGkgc3RFdnQ6YWN0aW9uPSJzYXZlZCIgc3RFdnQ6aW5zdGFuY2VJRD0ieG1wLmlpZDo2MjU2NDJkZC1iMGU4LWM3NGItOWIzNy1mMWYyMDg1OTlhNDAiIHN0RXZ0OndoZW49IjIwMjUtMDktMTRUMjE6NTM6NTgrMDg6MDAiIHN0RXZ0OnNvZnR3YXJlQWdlbnQ9IkFkb2JlIFBob3Rvc2hvcCBDQyAoV2luZG93cykiIHN0RXZ0OmNoYW5nZWQ9Ii8iLz4gPC9yZGY6U2VxPiA8L3htcE1NOkhpc3Rvcnk+IDwvcmRmOkRlc2NyaXB0aW9uPiA8L3JkZjpSREY+IDwveDp4bXBtZXRhPiA8P3hwYWNrZXQgZW5kPSJyIj8+8+oN7QAAACdJREFUSIntzEEBAAAEBDD073xSeNkCrJPUjTl61Wq1Wq1Wq9Uv6gUkjQM5UA8iRAAAAABJRU5ErkJggg==";

class AssetsManager
{
private:
    /**
     * @brief 资产根目录
     */
    std::string _root;
    /**
     * @brief 资产映射表
     */
    std::unordered_map<std::string, Asset> _assetsMap;
    /**
     * @brief 最大加载数量
     */
    int MAX_LOAD_COUNT = 30;
    void _initRoot();

public:
    AssetsManager();
    ~AssetsManager();
};