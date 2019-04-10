//
// Created by zw on 2019/3/27.
//

#ifndef AUDIOCOURSE_VIDEOPLAYER_H
#define AUDIOCOURSE_VIDEOPLAYER_H

#include "QueueUtil.h"
#include "CallbackUtil.h"
extern "C"{
#include "libavcodec/avcodec.h"
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavutil/time.h>
};
class VideoPlayer{
public:
    int streamIndex=-1;
    AVCodecContext *avCodecContext=NULL;
    AVCodecParameters *codecPar=NULL;
    QueueUtil *videoQueue=NULL;
    PlayStatusUtil *playStatus=NULL;
    CallbackUtil *callbackUtil=NULL;
    AVRational time_base;
    pthread_t thread_play;

    VideoPlayer(PlayStatusUtil *playStatusUtil,CallbackUtil *callbackUtil) ;
    ~VideoPlayer();

    void play();
    void release();
};



#endif //AUDIOCOURSE_VIDEOPLAYER_H
