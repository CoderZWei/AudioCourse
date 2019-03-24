package com.example.zw.audiocourse;

import android.support.v4.app.NavUtils;

import com.example.zw.audiocourse.listener.OnInitListener;
import com.example.zw.audiocourse.listener.OnLoadListener;
import com.example.zw.audiocourse.listener.OnPauseResumeListener;
import com.example.zw.audiocourse.listener.OnTimeUpdateListener;

public class MyPlayer {

    private FfmpegWrapper ffmpegWrapper=null;

    public void init(String audioPath) {
        ffmpegWrapper=FfmpegWrapper.getWrapper();
        ffmpegWrapper.init(audioPath);

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
    public void pause() {
        ffmpegWrapper.pause();
    }

    public void resume() {
        ffmpegWrapper.resume();
    }
}
