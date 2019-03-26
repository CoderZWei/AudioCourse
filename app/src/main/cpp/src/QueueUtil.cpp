//
// Created by zw on 2019/3/13.
//

#include <pthread.h>
#include "QueueUtil.h"

QueueUtil::~QueueUtil() {

}

QueueUtil::QueueUtil(PlayStatusUtil *playStatusUtil) {
    this->playStatusUtil=playStatusUtil;
    pthread_mutex_init(&this->mutexPacket,NULL);
    pthread_cond_init(&this->condPacket,NULL);
}

void QueueUtil::putAVPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);//加锁
    this->queuePacket.push(packet);
    pthread_cond_signal(&this->condPacket);//释放信号量
    pthread_mutex_unlock(&mutexPacket);//解锁
}

int QueueUtil::getAVPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);//加锁
    while (this->playStatusUtil!=NULL && this->playStatusUtil->getStatus()!= false){
        if(this->queuePacket.size()>0){
            AVPacket *avPacket=this->queuePacket.front();
            //产生一个新的引用，第一个参数是dst,第二个参数是src,返回0表示成功
            if(av_packet_ref(packet,avPacket)==0){
                this->queuePacket.pop();
            }
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;
            break;
        } else{
            pthread_cond_wait(&condPacket,&mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);//解锁
    return 0;
}

int QueueUtil::getQueueSize() {
    int size=0;
    pthread_mutex_lock(&mutexPacket);
    size=this->queuePacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}

void QueueUtil::clearAVPacket() {
   // pthread_cond_signal(&condPacket);
    pthread_mutex_lock(&mutexPacket);
    while (!queuePacket.empty()){
        AVPacket *packet=queuePacket.front();
        queuePacket.pop();
        av_packet_free(&packet);
        av_free(packet);
        packet=NULL;
    }
    pthread_mutex_unlock(&mutexPacket);
}
