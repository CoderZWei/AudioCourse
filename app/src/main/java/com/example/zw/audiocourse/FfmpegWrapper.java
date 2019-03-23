package com.example.zw.audiocourse;

import android.util.Log;

import com.example.zw.audiocourse.listener.OnInitListener;

public class FfmpegWrapper {
    static {
        System.loadLibrary("native-lib");
    }
    private static FfmpegWrapper ffmpegWrapper;
    private OnInitListener onInitListener;
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
    public void start(){
        Thread thread=new Thread(new Runnable() {
            @Override
            public void run() {
                cpp_start();
            }
        });
        thread.start();
    }

    public native void cpp_init(String path);
    public native void cpp_start();


    public void setOnInitListener(OnInitListener onInitListener) {
        this.onInitListener = onInitListener;
    }
}
