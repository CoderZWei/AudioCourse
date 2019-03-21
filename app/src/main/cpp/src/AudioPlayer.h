//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_AUDIOPLAYER_H
#define AUDIOCOURSE_AUDIOPLAYER_H

#include <cstdio>
#include "QueueUtil.h"

extern "C"{
#include "libavcodec/avcodec.h"
};

class AudioPlayer{
public:
    AudioPlayer(PlayStatusUtil *pUtil);

    int streamIndex=-1;
    AVCodecContext *avCodecContext=NULL;
    AVCodecParameters *codecPar=NULL;
    QueueUtil *audioQueue=NULL;
    PlayStatusUtil *playStatus=NULL;

    AudioPlayer();
    ~AudioPlayer();



};
#endif //AUDIOCOURSE_AUDIOPLAYER_H
