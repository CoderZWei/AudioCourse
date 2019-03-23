package com.example.zw.audiocourse;

import com.example.zw.audiocourse.listener.OnInitListener;

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

}
