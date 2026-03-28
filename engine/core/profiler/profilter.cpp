#include "profiler.h"

#include <sstream>
#include <iomanip>

#include "../../boo.h"
#include "../gfx/gfx.h"
#include "../gfx/gfx-mgr.h"
#include "../gfx/base/gfx-render-texture.h"


namespace Boo
{
    Profiler::Profiler()
    {
    }
    void Profiler::init()
    {
        this->_uuid = UuidUtil::generateUUID();
        this->_width = 150;
        this->_height = 200;
        this->_matView = Mat4::identity();
        this->_matProj = Mat4::identity();
        this->_matProj.setM00(2.0f / (float)this->_width);
        this->_matProj.setM11(2.0f / (float)this->_height);
        //渲染性能分析窗口
        /*this->_renderTexture = new GfxRenderTexture(this->_uuid, this->_width*5, this->_height*5);
        GfxMgr::getInstance()->initRenderQueue(this->_uuid, this->_renderTexture);*/
        // // 背景材质
        // std::cout << "Profiler::init() bgMltAsset uuid: 1" <<  std::endl;
        // this->_bgMltAsset = new MaterialAsset();
        // MaterialAsset *mtl = dynamic_cast<MaterialAsset *>(assetsManager->getAsset(AssetBuiltinMaterial::UI));
        // this->_bgMltAsset->create(mtl);
        // this->_bgMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_bgMat = Mat4::identity();
        // this->_bgMat.scale(this->_width, this->_height, 1.0f);
        // //FPS
        // this->_fpsMltAsset = new MaterialAsset();
        // this->_fpsMltAsset->create(mtl);
        // this->_fpsMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_fpsMat = Mat4::identity();
        //  std::cout << "Profiler::init() bgMltAsset uuid: 2" <<  std::endl;
        // //上一帧
        // this->_lastMltAsset = new MaterialAsset();
        // this->_lastMltAsset->create(mtl);
        // this->_lastMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_lastFrameMat = Mat4::identity();
        // // 上一帧渲染时间
        // this->_lastTimeMltAsset = new MaterialAsset();
        // this->_lastTimeMltAsset->create(mtl);
        // this->_lastTimeMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_lastTimeMat = Mat4::identity();
        // // 上一帧渲染提交次数
        // this->_lastCountMltAsset = new MaterialAsset();
        // this->_lastCountMltAsset->create(mtl);
        // this->_lastCountMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_lastCountMat = Mat4::identity();

        // // 当前帧
        // this->_currentMltAsset = new MaterialAsset();
        // this->_currentMltAsset->create(mtl);
        // this->_currentMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_currentMat = Mat4::identity();
        // // 当前渲染物体数量
        // this->_currentCountMltAsset = new MaterialAsset();
        // this->_currentCountMltAsset->create(mtl);
        // this->_currentCountMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_currentCountMat = Mat4::identity();
        // // 当前帧逻辑时间
        // this->_logicTimeMltAsset = new MaterialAsset();
        // this->_logicTimeMltAsset->create(mtl);
        // this->_logicTimeMltAsset->setTexture(AssetBuiltinTexture::Default);
        // this->_logicTimeMat = Mat4::identity();

        // this->_objectCount = 0;
    }

    void Profiler::openDebug()
    {
        this->_isShowDebug = true;
    }
    void Profiler::closeDebug()
    {
        this->_isShowDebug = false;
    }

    void Profiler::logicStartTime()
    {
        this->_logicStartTime = TimeUtil::nowTime();
    }

    void Profiler::logicEndTime()
    {
        this->_logicTime = TimeUtil::nowTime() - this->_logicStartTime;
    }
    void Profiler::addObjectCount(int count)
    {
        this->_objectCount += count;
    }
    void Profiler::render()
    {
        if(!this->_isShowDebug){
            return;
        }

        this->_matView.identity();
        float scaleX =this->_width/(float)view->getWidth();
        float scaleY =this->_height/(float)view->getHeight();
        float posX = (-view->getWidth() * 0.5f + this->_width * 0.5f )*scaleX;
        float posY = (-view->getHeight() * 0.5f + this->_height* 0.5f )*scaleY;
        this->_matView.scale(scaleX, scaleY, 1.0f);
        this->_matView.translate(posX, posY, 0.0f);

        GfxMgr::getInstance()->submitRenderData(this->_uuid, this->_matView.data(), this->_matProj.data(), true);
        // 渲染背景
        this->_submitRenderObject(this->_bgMltAsset, this->_bgMat,1.0,1.0,1.0,0.5);
        // 渲染FPS
        this->_refreshFps();
        // 渲染上一帧
        this->_refreshLast();
        // 上一帧渲染时间
        this->_refreshLastTime();
        // 上一帧渲染提交次数
        this->_refreshLastCount();
        // 当前帧逻辑时间,渲染物体数量,绘制次数
        this->_refreshCurrent();
        this->_refreshCurrentCount();
        this->_refreshLogicTime();
      
    }
    void Profiler::_submitRenderObject(MaterialAsset *materialAsset, Mat4 &mat,float colorR,float colorG,float colorB,float colorA)
    {
        std::vector<float> _instanceData;
        _instanceData.reserve(16);
       _instanceData.insert(_instanceData.end(), mat.data().begin(), mat.data().end());
       _instanceData.insert(_instanceData.end(), {colorR,colorG,colorB,colorA});
    //    GfxMgr::getInstance()->submitRenderObject(this->_uuid,materialAsset->getGfxMaterial(), Gfx::_uiTestMesh, _instanceData);
    }
    void Profiler::_refreshFps()
    {
       
        this->_refresMsgData("FPS:60",24,this->_fpsMltAsset,this->_fpsMat,this->_fpsFontTexture,0);
        this->_submitRenderObject(this->_fpsMltAsset, this->_fpsMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshLast()
    {
        this->_refresMsgData("上一帧",24,this->_lastMltAsset,this->_lastFrameMat,this->_lastFontTexture,1);
        this->_submitRenderObject(this->_lastMltAsset, this->_lastFrameMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshLastTime()
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << Gfx::_renderTime;
        std::string result = oss.str();
        this->_refresMsgData("Time:"+result+"ms",24,this->_lastTimeMltAsset,this->_lastTimeMat,this->_lastTimeFontTexture,2);
        this->_submitRenderObject(this->_lastTimeMltAsset, this->_lastTimeMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshLastCount()
    {
        this->_refresMsgData("DrawCall:"+std::to_string(Gfx::_drawCount), 24, this->_lastCountMltAsset, this->_lastCountMat, this->_lastCountFontTexture, 3);
        this->_submitRenderObject(this->_lastCountMltAsset, this->_lastCountMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshCurrent(){
        this->_refresMsgData("当前帧",24,this->_currentMltAsset,this->_currentMat,this->_currentFontTexture,4);
        this->_submitRenderObject(this->_currentMltAsset, this->_currentMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshCurrentCount()
    {
        this->_refresMsgData("Count:"+std::to_string(this->_objectCount), 24, this->_currentCountMltAsset, this->_currentCountMat, this->_currentCountFontTexture, 5);
        this->_submitRenderObject(this->_currentCountMltAsset, this->_currentCountMat,1.0,0.0,0.0,1.0);
    }
    void Profiler::_refreshLogicTime()
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << (this->_logicTime/1000.0f/1000.0f);
        std::string result = oss.str();
        this->_refresMsgData("LogicTime:"+result+"ms",24,this->_logicTimeMltAsset,this->_logicTimeMat,this->_logicTimeFontTexture,6);
        this->_submitRenderObject(this->_logicTimeMltAsset, this->_logicTimeMat,1.0,0.0,0.0,1.0);
    }


    void Profiler::_refresMsgData(std::string text,int size,MaterialAsset *materialAsset,Mat4 &mat,FontTexture &fontTexture,int index)
    {
        // if(fontTexture.text==text){
        //     return;
        // }
        // // std::cout<<"text:"<<text<<std::endl;
        // GfxMgr::getInstance()->destroyTexture(fontTexture.gfxUuid);
        // FreetypeMgr::getInstance()->create(text,fontTexture);
        // materialAsset->setTexture(fontTexture.gfxUuid);
        // float width=fontTexture.width*(size/50.0f);
        // float height=fontTexture.height*(size/50.0f);
        // float x =(-this->_width*0.5f+10)+width*0.5;
        // float y = (this->_height*0.5f-height*0.5)-index*3.0-height*index;
        // // std::cout<<"x:"<<x<<" y:"<<y<<std::endl;
        // // std::cout<<"width:"<<width<<" height:"<<height<<std::endl;
        // mat.scale(width, height, 1.0f);
        // mat.translate(x, y, 0.0f);
    }


   float Profiler::getLogicTime()
    {
        return this->_logicTime;
    }
    int Profiler::getObjectCount()
    {
        return this->_objectCount;
    }
    void Profiler::clear()
    {
        this->_logicTime = 0.0f;
        this->_objectCount = 0;
    }

  
} // namespace Boo
