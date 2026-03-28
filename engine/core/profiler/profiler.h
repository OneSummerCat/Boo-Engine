#pragma once
#include <iostream>
#include <string>
#include "../util/util-api.h"
#include "../math/math-api.h"
#include "../font/freetype-mgr.h"

class GfxRenderTexture;
namespace Boo
{
    class MaterialAsset;
    struct FontTexture;
}
namespace Boo
{
    class Profiler
    {
    private:
        bool _isShowDebug = false;
        // 当前帧统计
        long long _logicStartTime = 0;
        float _logicTime = 0.0f;
        int _objectCount = 0;
    private:
        std::string _uuid = "";
        int _width = 0;
        int _height = 0;

        /**
         * 视图矩阵
         * 将3D世界空间 → 3D相机空间
         */
        Mat4 _matView = Mat4::identity();
        /**
         * 投影矩阵
         * 将3D观察空间 → 2D裁剪空间（NDC空间：-1到1）
         */
        Mat4 _matProj = Mat4::identity();
        GfxRenderTexture *_renderTexture = nullptr;
        // 背景
        MaterialAsset *_bgMltAsset = nullptr;
        Mat4 _bgMat = Mat4::identity();
        //FPS
        MaterialAsset *_fpsMltAsset = nullptr;
        Mat4 _fpsMat = Mat4::identity();
        FontTexture _fpsFontTexture;

        // 上一帧
        MaterialAsset *_lastMltAsset = nullptr;
        Mat4 _lastFrameMat = Mat4::identity();
        FontTexture _lastFontTexture;
        // 上一帧渲染时间
        MaterialAsset *_lastTimeMltAsset = nullptr;
        Mat4 _lastTimeMat = Mat4::identity();
        FontTexture _lastTimeFontTexture;
        // 上一帧渲染提交次数
        MaterialAsset *_lastCountMltAsset = nullptr;
        Mat4 _lastCountMat = Mat4::identity();
        FontTexture _lastCountFontTexture;
        // 当前帧
        MaterialAsset *_currentMltAsset = nullptr;
        Mat4 _currentMat = Mat4::identity();
        FontTexture _currentFontTexture;
        // 当前渲染物体数量
        MaterialAsset *_currentCountMltAsset = nullptr;
        Mat4 _currentCountMat = Mat4::identity();
        FontTexture _currentCountFontTexture;
        // 逻辑时间
        MaterialAsset *_logicTimeMltAsset = nullptr;
        Mat4 _logicTimeMat = Mat4::identity();
        FontTexture _logicTimeFontTexture;

        void _refreshFps();
        void _refreshLast();
        void _refreshLastTime();
        void _refreshLastCount();
        void _refreshCurrent();
        void _refreshCurrentCount();
        void _refreshLogicTime();

        void _refresMsgData(std::string text,int size,MaterialAsset *materialAsset,Mat4 &mat,FontTexture &fontTexture,int index);
        void _submitRenderObject(MaterialAsset *materialAsset,Mat4 &mat,float colorR,float colorG,float colorB,float colorA);

    public:
        Profiler();
        void init();
        void openDebug();
        void closeDebug();
        void logicStartTime();
        void logicEndTime();
        void addObjectCount(int count);
        void render();
        void clear();
        float getLogicTime();
        int getObjectCount();

        ~Profiler();
    };
}