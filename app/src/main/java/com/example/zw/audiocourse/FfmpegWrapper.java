package com.example.zw.audiocourse;

import android.util.Log;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;
import com.example.zw.audiocourse.listener.OnTimeUpdateListener;
import com.example.zw.audiocourse.opengl.MyGLSurfaceView;
import com.example.zw.audiocourse.util.TimeInfoBean;
import com.example.zw.audiocourse.util.TimeUtil;

import java.util.TreeMap;

public class FfmpegWrapper {
    static {
        System.loadLibrary("native-lib");
    }
    private static FfmpegWrapper ffmpegWrapper;
    private OnInitListener onInitListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
    private TimeInfoBean timeInfoBean=null;
    private static String audioSource;//音频数据源
    private static boolean playSwitch=false;
    private MyGLSurfaceView mGLSurfaceView;

    private OnTimeUpdateListener onTimeUpdateListener;
    public static FfmpegWrapper getWrapper(){
        if(ffmpegWrapper==null){
            synchronized (FfmpegWrapper.class){
                ffmpegWrapper=new FfmpegWrapper();
            }
        }
        return ffmpegWrapper;
    }
    public void init(final String path){
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run() {
                audioSource=path;
                cpp_init(audioSource);
            }
        });
        thread.start();
    }
    public void setGLSurfaceView(MyGLSurfaceView mGLSurfaceView) {
        this.mGLSurfaceView = mGLSurfaceView;
    }
    //C++回调
    public void onCallInit(){
        if(this.onInitListener!=null){
            onInitListener.onInited();
        }
    }
    public void onCallLoad(boolean load){
        //Log.d("zw_cal","here");
        if(this.onLoadListener!=null){
            onLoadListener.onLoad(load);
        }
    }
    public void onCallTimeUpdate(int currentTime,int totalTime){
        if(this.onTimeUpdateListener!=null){
            if(timeInfoBean==null){
                timeInfoBean=new TimeInfoBean();
            }
            timeInfoBean.setCurrentTime(currentTime);
            timeInfoBean.setTotalTime(totalTime);
            onTimeUpdateListener.onTimeUpdate(timeInfoBean);
        }
    }
    public void onCallSwitch(){
        if(playSwitch){
            playSwitch=false;
            init(audioSource);
        }
    }
    public void onCallRenderYUV(int width,int height,byte[]y,byte[]u,byte[]v){
        if(mGLSurfaceView!=null){
            mGLSurfaceView.setYUVData(width, height, y, u, v);
        }
        Log.d("ZW_CALLBACK","获取到yuv");
    }
    public void setOnInitListener(OnInitListener onInitListener) {
        this.onInitListener = onInitListener;
    }

    public void setOnLoadListener(OnLoadListener onLoadListener) {
        this.onLoadListener = onLoadListener;
    }

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener) {
        this.onPauseResumeListener = onPauseResumeListener;
    }

    public void start(){
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_start();
            }
        });
        thread.start();
    }
    public void setOnTimeUpdateListener(OnTimeUpdateListener onTimeUpdateListener) {
        this.onTimeUpdateListener = onTimeUpdateListener;
    }

    public void pause() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_pause();
                onPauseResumeListener.onPause(true);
            }
        }).start();
    }
    public void resume() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_resume();
                onPauseResumeListener.onPause(false);
            }
        }).start();
    }
    public native void cpp_init(String path);
    public native void cpp_start();
    public native void cpp_pause();
    public native void cpp_resume();
    public native void cpp_stop();
    public native void cpp_seek(int time_sec);



    public void stop() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_stop();
            }
        }).start();
    }

    public void seek(int time_sec) {
        cpp_seek(time_sec);
    }

    public void switchAudio(String audioUrl) {
        audioSource=audioUrl;
        playSwitch=true;
        stop();
    }


}
