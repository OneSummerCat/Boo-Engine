package com.booengine.sdk;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class BooEngineSdk implements SurfaceHolder.Callback{

    private static BooEngineSdk instance;
    private static final String TAG = "BooEngine";
    private BooEngineSdk(){}
    public static BooEngineSdk getInstance(){
        if(instance==null){
            instance=new BooEngineSdk();
        }
        return  instance;
    }
    // 加载Native库
    static {
        try {
            System.loadLibrary("BooEngine");  // 对应 libBooEngine.so
            Log.i(TAG, "SDK:Native library loaded successfully");
        } catch (ArithmeticException e ) {
            Log.i(TAG, "SDK:Failed to load native library", e);
        }
    }

    private RenderThread renderThread;




    public void init(SurfaceView surfaceView){
        Log.i(TAG,"SDK:init");
        SurfaceHolder surfaceHolder= surfaceView.getHolder();
        surfaceHolder.addCallback(this);
        renderThread=new RenderThread();
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        Log.i(TAG,"SDK:surface created");
        Surface suface=surfaceHolder.getSurface();
        int width = surfaceHolder.getSurfaceFrame().width();
        int height = surfaceHolder.getSurfaceFrame().height();
        renderThread.engineInit(suface,width,height);
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int format, int width, int height) {
        Log.i(TAG,"SDK:surface changed");
        renderThread.engineResize(width, height);
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        Log.i(TAG,"SDK:surface destroy");
        // 停止渲染线程
        renderThread.engineDestory();
        try {
            renderThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * 开启一个渲染线程
     */
    private class RenderThread extends Thread {
        private final Object pauseLock = new Object();
        private Surface surface;
        private int width;
        private int height;

        RenderThread(){
            this.surface=null;
            this.width=0;
            this.height=0;
        }

        public void engineInit(Surface surface,int width,int height) {
            Log.i(TAG,"SDK:render thread init");
            this.surface = surface;
            this.width=width;
            this.height=height;
            start();
        }
        public void engineResize(int width,int height){
            Log.i(TAG,"SDK:render thread resize");
            EngineResize(width,height);
        }
        public void enginePause(){

        }
        public void engineResume(){

        }
        public void engineDestory(){
            Log.i(TAG,"SDK:render thread destory");
            EngineDestory();
        }
        @Override
        public void run() {
            EngineInit(this.surface,this.width,this.height);
            while(true){
                // 处理暂停
                synchronized (pauseLock) {
//                    while (isPaused && isRunning) {
//                        try {
//                            pauseLock.wait();
//                        } catch (InterruptedException e) {
//                            e.printStackTrace();
//                        }
//                    }
                }
                EngineTick();
                // 控制帧率（可选）
                try {
                    Thread.sleep(16); // 约60fps
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

//            // 清理Native资源
//            nativeCleanup();
        }
    }
    /**
     * native 函数定义
     */
    private static native void EngineInit(Surface surface,int width, int height);
    private static native void EngineResize(int width, int height);
    private static native void EngineTick();
    private static native void EngineDestory();
}
