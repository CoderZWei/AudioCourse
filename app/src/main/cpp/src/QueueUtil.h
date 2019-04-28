//
// Created by zw on 2019/3/13.
//

#ifndef AUDIOCOURSE_QUEUEUTIL_H
#define AUDIOCOURSE_QUEUEUTIL_H

#include "queue"
#include "PlayStatusUtil.h"
extern "C"{
#include <libavcodec/avcodec.h>
};


class QueueUtil{
public:
    //QueueUtil();

    std::queue<AVPacket*>queuePacket;
    pthread_mutex_t mutexPacket;
    pthread_cond_t condPacket;
    PlayStatusUtil *playStatusUtil=NULL;

    void putAVPacket(AVPacket *packet);
    int getAVPacket(AVPacket *packet);
    int getQueueSize();
    void clearAVPacket();
    void noticeQueue();
    QueueUtil(PlayStatusUtil *playStatusUtil);
    ~QueueUtil();

};
#endif //AUDIOCOURSE_QUEUEUTIL_H
