package com.example.zw.audiocourse;

import android.util.Log;

public class FfmpegWrapper {
    static {
        System.loadLibrary("native-lib");
    }
    private static FfmpegWrapper ffmpegWrapper;
    public static FfmpegWrapper getWrapper(){
        if(ffmpegWrapper==null){
            synchronized (FfmpegWrapper.class){
                ffmpegWrapper=new FfmpegWrapper();
            }
        }
        return ffmpegWrapper;
    }
    public void init(String path){
        Log.e("zw:PATH_",path);
        cpp_init(path);
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
}
