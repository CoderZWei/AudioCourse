//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_AUDIOPLAYER_H
#define AUDIOCOURSE_AUDIOPLAYER_H

#include <cstdio>
#include "QueueUtil.h"

extern "C"{
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
};

class AudioPlayer{
public:
    AudioPlayer(PlayStatusUtil *pUtil);

    int streamIndex=-1;
    AVCodecContext *avCodecContext=NULL;
    AVCodecParameters *codecPar=NULL;
    QueueUtil *audioQueue=NULL;
    PlayStatusUtil *playStatus=NULL;
    pthread_t thread_play;
    AVPacket *avPacket=NULL;
    AVFrame *avFrame=NULL;
    int ret=0;
    uint8_t  *buffer=NULL;
    int data_size=0;


    AudioPlayer();
    ~AudioPlayer();
    void play();
    void resampleAudio();



};
#endif //AUDIOCOURSE_AUDIOPLAYER_H
