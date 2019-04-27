//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_FFMEPGPLAYER_H
#define AUDIOCOURSE_FFMEPGPLAYER_H

#include "pthread.h"
#include "AudioPlayer.h"
#include "CallbackUtil.h"
#include "VideoPlayer.h"
extern "C"{
    //#include "include/libavformat/avformat.h"
    //#include "libavformat/avformat.h"
#include "libavformat/avformat.h"
#include <libavutil/time.h>
};
class FfmpegPlayer{
    public:
        FfmpegPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil);
        //音频相关
        const char *url=NULL;
        AudioPlayer *audioPlayer=NULL;
        pthread_t decodeThread;
        AVFormatContext *pFormatCtx=NULL;
        PlayStatusUtil *playStatus=NULL;
        CallbackUtil *callbackUtil=NULL;
        pthread_mutex_t init_mutex;
        pthread_mutex_t seek_mutex;
        int duration=0;
        bool exit= false;
        //视频相关
        VideoPlayer *videoPlayer=NULL;
        bool supportMediaCodec= false;
        const AVBitStreamFilter *bsFilter=NULL;

        FfmpegPlayer();
        ~FfmpegPlayer();
        void init(const char *url);
        void start();
        void startDecode();
        void pause();
        void resume();
        void release();
        void seek(int64_t time_sec);
        //要用二级指针，因为avCodecContext是在局部函数里新建的
        int getCodecContext(AVCodecParameters *codecpar,AVCodecContext **avCodecContext);




        //const char* url= NULL;

};


#endif //AUDIOCOURSE_FFMEPGPLAYER_H
