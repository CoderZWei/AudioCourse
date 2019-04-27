package com.example.zw.audiocourse;

import android.support.v4.app.NavUtils;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;
import com.example.zw.audiocourse.listener.OnTimeUpdateListener;
import com.example.zw.audiocourse.opengl.MyGLSurfaceView;

public class MyPlayer {

    private FfmpegWrapper ffmpegWrapper=null;
    private int duration=0;

    public void init(final String audioPath) {
        ffmpegWrapper=FfmpegWrapper.getWrapper();

        new Thread(new Runnable() {
            @Override
            public void run() {
                ffmpegWrapper.init(audioPath);
            }
        }).start();
    }

    public void start() {
        ffmpegWrapper.start();
    }

    public void setOnInitListener(OnInitListener onInitListener) {
        if(ffmpegWrapper==null){
            ffmpegWrapper=FfmpegWrapper.getWrapper();
        }
        ffmpegWrapper.setOnInitListener(onInitListener);
    }
    public void setOnLoadListener(OnLoadListener onLoadListener) {
        if(ffmpegWrapper==null){
            ffmpegWrapper=FfmpegWrapper.getWrapper();
        }
        ffmpegWrapper.setOnLoadListener(onLoadListener);
    }

    public void setOnPauseResumeListener(OnPauseResumeListener onPauseResumeListener) {
        if(ffmpegWrapper==null){
            ffmpegWrapper=FfmpegWrapper.getWrapper();
        }
        ffmpegWrapper.setOnPauseResumeListener(onPauseResumeListener);
    }
    public void setOnTimeUpdateListener(OnTimeUpdateListener onTimeUpdateListener) {
        if(ffmpegWrapper==null){
            ffmpegWrapper=FfmpegWrapper.getWrapper();
        }
        ffmpegWrapper.setOnTimeUpdateListener(onTimeUpdateListener);
    }
    public void setGLSurfaceView(MyGLSurfaceView mGLSurfaceView) {
        if(ffmpegWrapper==null){
            ffmpegWrapper=FfmpegWrapper.getWrapper();
        }
        ffmpegWrapper.setGLSurfaceView(mGLSurfaceView);
    }

    public void pause() {
        ffmpegWrapper.pause();
    }

    public void resume() {
        ffmpegWrapper.resume();
    }

    public void stop() {
        ffmpegWrapper.stop();
    }
    public void switchAudio(String audioUrl) {
        ffmpegWrapper.switchAudio(audioUrl);
    }
    public void seek(int time_sec) {
        ffmpegWrapper.seek(time_sec);
    }
    public int getDuration(){
        return ffmpegWrapper.getDurationTime();
    }

}
