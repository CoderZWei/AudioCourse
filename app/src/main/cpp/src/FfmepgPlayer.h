//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_FFMEPGPLAYER_H
#define AUDIOCOURSE_FFMEPGPLAYER_H

#include "pthread.h"
#include "AudioPlayer.h"
#include "CallbackUtil.h"
extern "C"{
    //#include "include/libavformat/avformat.h"
    //#include "libavformat/avformat.h"
#include "libavformat/avformat.h"
#include <libavutil/time.h>
};
class FfmpegPlayer{
    public:
        FfmpegPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil);

        const char *url=NULL;
        AudioPlayer *audioPlayer=NULL;
        pthread_t decodeThread;
        AVFormatContext *pFormatCtx=NULL;
        PlayStatusUtil *playStatus=NULL;
        CallbackUtil *callbackUtil=NULL;
        pthread_mutex_t init_mutex;
        bool exit= false;

        FfmpegPlayer();
        ~FfmpegPlayer();
        void init(const char *url);
        void start();
        void startDecode();
        void pause();
        void resume();
        void release();





        //const char* url= NULL;

};


#endif //AUDIOCOURSE_FFMEPGPLAYER_H
