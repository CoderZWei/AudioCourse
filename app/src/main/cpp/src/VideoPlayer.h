//
// Created by zw on 2019/3/27.
//

#ifndef AUDIOCOURSE_VIDEOPLAYER_H
#define AUDIOCOURSE_VIDEOPLAYER_H

#include "QueueUtil.h"
#include "CallbackUtil.h"
#include "AudioPlayer.h"
#define CODEC__YUV 0
#define CODEC_MEDIACODEC 1
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

    AudioPlayer *audioPlayer=NULL;
    double clock=0;
    double delayTime=0;
    double defaultDelayTime=0.04;

    pthread_mutex_t codecMutex;
    int codecType=CODEC__YUV;
    AVBSFContext *abs_ctx=NULL;

    VideoPlayer(PlayStatusUtil *playStatusUtil,CallbackUtil *callbackUtil) ;
    ~VideoPlayer();

    void play();
    double getFrameDiffTime(AVFrame *avFrame,AVPacket *avPacket);
    double getDelayTime(double diff);
    void release();

};



#endif //AUDIOCOURSE_VIDEOPLAYER_H
