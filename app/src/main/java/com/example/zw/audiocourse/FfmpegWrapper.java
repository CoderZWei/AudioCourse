package com.example.zw.audiocourse;

import android.util.Log;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;

import java.util.TreeMap;

public class FfmpegWrapper {
    static {
        System.loadLibrary("native-lib");
    }
    private static FfmpegWrapper ffmpegWrapper;
    private OnInitListener onInitListener;
    private OnLoadListener onLoadListener;
    private OnPauseResumeListener onPauseResumeListener;
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
                cpp_init(path);
            }
        });
        thread.start();
    }
    //C++回调
    public void onCallInit(){
        if(this.onInitListener!=null){
            onInitListener.onInited();
        }
    }
    public void onCallLoad(boolean load){
        Log.d("zw_cal","here");
        if(this.onLoadListener!=null){
            onLoadListener.onLoad(load);
        }
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


}
