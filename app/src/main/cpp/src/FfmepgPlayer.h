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
};
class FfmpegPlayer{
    public:
    FfmpegPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil);

    const char *url=NULL;
        AudioPlayer *audioPlayer=NULL;
        pthread_t decodeThread;
        AVFormatContext *pFormaxCtx=NULL;
        PlayStatusUtil *playStatus=NULL;
        CallbackUtil *callbackUtil=NULL;

        FfmpegPlayer();
        ~FfmpegPlayer();
        void init(const char *url);
        void start();
        void startDecode();





        //const char* url= NULL;

};


#endif //AUDIOCOURSE_FFMEPGPLAYER_H
