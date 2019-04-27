//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_AUDIOPLAYER_H
#define AUDIOCOURSE_AUDIOPLAYER_H

#include <cstdio>
#include "QueueUtil.h"
#include "CallbackUtil.h"

extern "C"{
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include <libavutil/time.h>
};

class AudioPlayer{
public:
    AudioPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil,int sampleRate);

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
    int dataSize=0;
    int sampleRate=0;
    CallbackUtil *callbackUtil=NULL;
    //OpenSL ES相关变量
     //引擎接口
     SLObjectItf engineObject=NULL;
     SLEngineItf engineEngine=NULL;
     //混音器
     SLObjectItf outputMixObject=NULL;
     SLEnvironmentalReverbItf outputMixEnvironmentalReverb=NULL;
     SLEnvironmentalReverbSettings reverbSettings=SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
     //pcm
     SLObjectItf  pcmPlayerObject=NULL;
     SLPlayItf pcmPlayerPlay=NULL;
     //缓冲器队列接口
      SLAndroidSimpleBufferQueueItf pcmBufferQueue=NULL;
      //时间相关变量
      int duration=0;
      AVRational time_base;
      double clock;//总的播放时长
      double now_time;//当前播放时长
      double last_time;//上一次调用时间

      pthread_mutex_t codecMutex;


     AudioPlayer();
     ~AudioPlayer();
     void play();
     int resampleAudio();
     void initOpenSLES();
     int getCurrentSampleRateForOpenSLES(int sampleRate);

     void pause();
     void resume();
     void release();



};
#endif //AUDIOCOURSE_AUDIOPLAYER_H
