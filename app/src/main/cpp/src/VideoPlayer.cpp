//
// Created by zw on 2019/3/27.
//
#include <pthread.h>


#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(PlayStatusUtil *playStatusUtil,CallbackUtil *callbackUtil) {
    this->playStatus=playStatusUtil;
    this->callbackUtil=callbackUtil;
    this->videoQueue=new QueueUtil(playStatus);
    pthread_mutex_init(&codecMutex,NULL);
}

VideoPlayer::~VideoPlayer() {
    pthread_mutex_destroy(&codecMutex);
}
void *playVideo(void*data){
    VideoPlayer *videoPlayer= static_cast<VideoPlayer*>(data);
    while (videoPlayer->playStatus!=NULL && videoPlayer->playStatus->getPlayStatus()== true){
        //AVPacket *avPacket=av_packet_alloc();
       // if(videoPlayer->videoQueue->getAVPacket(avPacket)==0){//取packet成功
            //解码渲染

            if(videoPlayer->playStatus->getSeekStatus()== true) {
                //  ALOGD("zw_Video:here1");
                av_usleep(1000 * 100);
                continue;
            }
            if(videoPlayer->playStatus->getPauseStatus()== true){
                av_usleep(1000 * 100);
                continue;
            }
            if(videoPlayer->videoQueue->getQueueSize()==0){//加载状态
              //  ALOGD("zw_Video:here2");
                if(videoPlayer->playStatus->getLoadStatus()== false){
                    videoPlayer->playStatus->setLoadStatus(true);
                    videoPlayer->callbackUtil->onCallLoad(CHILD_THREAD, true);
                }
                av_usleep(1000*100);
                continue;
            } else{
                if(videoPlayer->playStatus->getLoadStatus()== true){
                    videoPlayer->playStatus->setLoadStatus(false);
                    videoPlayer->callbackUtil->onCallLoad(CHILD_THREAD, false);
                }
            }
           //ALOGD("zw_videoSize:%d",videoPlayer->videoQueue->getQueueSize());

           AVPacket *avPacket=av_packet_alloc();
            if(videoPlayer->videoQueue->getAVPacket(avPacket)!=0){//取packet失败
                //ALOGD("zw_Video:解码成功1");
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket=NULL;
                continue;
            }
            if(videoPlayer->codecType==CODEC_MEDIACODEC){
                //硬解码
                //ALOGD("硬解码视频");
                //5、通过过滤器对AVPacket处理
                if(av_bsf_send_packet(videoPlayer->abs_ctx,avPacket)!=0){
                    av_packet_free(&avPacket);
                    av_free(avPacket);
                    avPacket=NULL;
                    continue;
                }
                while (av_bsf_receive_packet(videoPlayer->abs_ctx,avPacket)==0){
                    ALOGD("开始硬解码");
                    double diff=videoPlayer->getFrameDiffTime(NULL,avPacket);
                    av_usleep(videoPlayer->getDelayTime(diff)*1000000);
                    //
                    videoPlayer->callbackUtil->onCallDecodeAVPacket(avPacket->size,avPacket->data);
                    av_packet_free(&avPacket);
                    av_free(avPacket);
                    continue;
                }
                avPacket=NULL;

            }else if(videoPlayer->codecType==CODEC__YUV){
                //软解码
                pthread_mutex_lock(&videoPlayer->codecMutex);
                if(avcodec_send_packet(videoPlayer->avCodecContext,avPacket)!=0){
                    // ALOGD("zw_Video:解码成功2");
                    av_packet_free(&avPacket);
                    av_free(avPacket);
                    avPacket=NULL;
                    pthread_mutex_unlock(&videoPlayer->codecMutex);
                    continue;
                }

                AVFrame *avFrame=av_frame_alloc();
                if(avcodec_receive_frame(videoPlayer->avCodecContext,avFrame)!=0){//返回0表示成功
                    ALOGD("zw_Video:解码失败");
                    av_frame_free(&avFrame);
                    av_free(avFrame);
                    avFrame=NULL;
                    av_packet_free(&avPacket);
                    av_free(avPacket);
                    avPacket=NULL;
                    pthread_mutex_unlock(&videoPlayer->codecMutex);
                    continue;
                }else{
                    ALOGD("zw_Video:解码成功");
                }
                //视频帧格式为yuv420p，则无需转换
                if(avFrame->format==AV_PIX_FMT_YUV420P){
                    //ALOGD("zw_frame:是yuv420p格式");
                    double diff=videoPlayer->getFrameDiffTime(avFrame,NULL);
                    av_usleep(videoPlayer->getDelayTime(diff) * 1000000);
                    videoPlayer->callbackUtil->onCallRenderYUV(
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            avFrame->data[0],
                            avFrame->data[1],
                            avFrame->data[2]
                    );
                }else{
                    //ALOGD("zw_frame:不是yuv420p格式");
                    AVFrame *pFrameYUV420P=av_frame_alloc();
                    //ALOGD("zw_width:%d height:%d",videoPlayer->avCodecContext->width,videoPlayer->avCodecContext->height);
                    int num=av_image_get_buffer_size(
                            AV_PIX_FMT_YUV420P,
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            1
                    );
                    //ALOGD("zw_num:%d",num);
                    uint8_t *buffer= static_cast<uint8_t *>(av_malloc(num* sizeof(uint8_t)));
                    av_image_fill_arrays(
                            pFrameYUV420P->data,
                            pFrameYUV420P->linesize,
                            buffer,
                            AV_PIX_FMT_YUV420P,
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            1
                    );
                    SwsContext *sws_ctx=sws_getContext(
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            videoPlayer->avCodecContext->pix_fmt,
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            AV_PIX_FMT_YUV420P,
                            SWS_BICUBIC,
                            NULL,NULL,NULL
                    );
                    //初始化失败
                    if(!sws_ctx){
                        ALOGD("zw_swsCtx初始化失败");
                        av_frame_free(&pFrameYUV420P);
                        av_free(pFrameYUV420P);
                        av_free(buffer);
                        pthread_mutex_unlock(&videoPlayer->codecMutex);
                        continue;
                    }else{
                        ALOGD("zw_swsCtx初始化成功");
                    }
                    sws_scale(sws_ctx,
                              reinterpret_cast<const uint8_t *const *>(avFrame->data),
                              avFrame->linesize,
                              0,
                              avFrame->height,
                              pFrameYUV420P->data,
                              pFrameYUV420P->linesize
                    );
                    videoPlayer->callbackUtil->onCallRenderYUV(
                            videoPlayer->avCodecContext->width,
                            videoPlayer->avCodecContext->height,
                            pFrameYUV420P->data[0],
                            pFrameYUV420P->data[1],
                            pFrameYUV420P->data[2]
                    );
                    av_frame_free(&pFrameYUV420P);
                    av_free(pFrameYUV420P);
                    av_free(buffer);
                    sws_freeContext(sws_ctx);
                }
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                pthread_mutex_unlock(&videoPlayer->codecMutex);
            }
    }
    //change
    return 0;
       // av_packet_free(&avPacket);
        //av_free(avPacket);
        //avPacket=NULL;
    //}

    //pthread_exit(&videoPlayer->thread_play);
}
void VideoPlayer::play() {
    if(playStatus!=NULL && playStatus->getPlayStatus()== true){
        pthread_create(&thread_play,NULL,playVideo,this);
    }
}

void VideoPlayer::release() {
    if(videoQueue!=NULL){
        videoQueue->noticeQueue();
    }
    pthread_join(thread_play,NULL);
    if(videoQueue!=NULL){
        delete(videoQueue);
        videoQueue=NULL;
    }
    if(abs_ctx!=NULL){
        av_bsf_free(&abs_ctx);
        abs_ctx=NULL;
    }
    if(avCodecContext!=NULL){
        pthread_mutex_destroy(&codecMutex);
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
        avCodecContext=NULL;
        pthread_mutex_unlock(&codecMutex);
    }
    if(playStatus!=NULL){
        playStatus=NULL;
    }
    if(callbackUtil!=NULL){
        callbackUtil=NULL;
    }
}

double VideoPlayer::getFrameDiffTime(AVFrame *avFrame,AVPacket *avPacket) {
    double pts=0;
    if(avFrame!=NULL){
        pts=av_frame_get_best_effort_timestamp(avFrame);
    }
    if(avPacket!=NULL){
        pts=avPacket->pts;
    }
    if(pts==AV_NOPTS_VALUE){
        pts=0;
    }
    pts*=av_q2d(time_base);
    if(pts>0){
        clock=pts;
    }
    double diff=audioPlayer->clock-clock;
    return diff;
}

double VideoPlayer::getDelayTime(double diff) {
    if(diff>0.003){//音频快于视频
        delayTime=delayTime*2/3;
        if(delayTime<defaultDelayTime/2){
            delayTime=defaultDelayTime*2/3;
        }else if(delayTime>defaultDelayTime*2){
            delayTime=defaultDelayTime*2;
        }
    }else if(diff<-0.003){//视频快于音频
        delayTime=delayTime*3/2;
        if(delayTime<defaultDelayTime/2){
            delayTime=defaultDelayTime*2/3;
        }else if(delayTime>defaultDelayTime*2){
            delayTime=defaultDelayTime*2;
        }
    }
    if(diff >= 0.5)
    {
        delayTime = 0;
    }
    else if(diff <= -0.5)
    {
        delayTime = defaultDelayTime * 2;
    }

    if(fabs(diff) >= 10)
    {
        delayTime = defaultDelayTime;
    }
    return delayTime;

}
