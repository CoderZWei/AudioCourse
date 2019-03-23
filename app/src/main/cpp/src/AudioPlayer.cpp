//
// Created by zw on 2019/3/13.
//

#include <pthread.h>
#include "AudioPlayer.h"
#include "log.h"

AudioPlayer::AudioPlayer() {

}

AudioPlayer::~AudioPlayer() {

}

AudioPlayer::AudioPlayer(PlayStatusUtil *pUtil) {
    this->playStatus=pUtil;
    this->audioQueue=new QueueUtil(playStatus);
    this->buffer=(uint8_t*)av_malloc(44100*2*2);
}
void *decodePlay(void*data){
    AudioPlayer *audioPlayer=(AudioPlayer*)data;
    audioPlayer->resampleAudio();
    pthread_exit(&audioPlayer->thread_play);
}
void AudioPlayer::play() {
    pthread_create(&thread_play,NULL,decodePlay,this);
}

void AudioPlayer::resampleAudio() {
    FILE *outFile=fopen("/storage/emulated/0/myMusic.pcm","w");
    if(outFile==NULL){
        ALOGD("zw:outfile is NULL");
    }

    while (playStatus!=NULL && playStatus->getStatus()== true){
        avPacket=av_packet_alloc();
        if(audioQueue->getAVPacket(avPacket)!=0){
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;
            continue;
        }
        ret=avcodec_send_packet(avCodecContext,avPacket);//返回0表示成功
        if(ret!=0){
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket=NULL;
            continue;
        }
        avFrame=av_frame_alloc();
        ret=avcodec_receive_frame(avCodecContext,avFrame);//返回0表示成功
        if(ret==0){
        	//处理异常情况 声道数和声道布局
            if(avFrame->channels>0 && avFrame->channel_layout==0){
                avFrame->channel_layout=av_get_default_channel_layout(avFrame->channels);
            } else if(avFrame->channels==0 && avFrame->channel_layout>0){
                avFrame->channels=av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }
            SwrContext *swr_ctx;
            swr_ctx=swr_alloc_set_opts(
                    NULL,
                    AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_S16,
                    avFrame->sample_rate,
                    avFrame->channel_layout,
                    (AVSampleFormat) avFrame->format,
                    avFrame->sample_rate,
                    NULL, NULL
                    );
            if(!swr_ctx || swr_init(swr_ctx)<0){
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;
                if(swr_ctx!=NULL){
                	swr_free(&swr_ctx);	
                	swr_ctx=NULL;
                }
                continue;
            }
            //返回值是转换后的PCM的采样个数
            int nb=swr_convert(
                    swr_ctx,
                    &buffer,
                    avFrame->nb_samples,
                    (const uint8_t **)avFrame->data,
                    avFrame->nb_samples
                    );
            int out_channels=av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size=nb*out_channels*av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
            ALOGD("zw_data_size:%d",data_size);
            fwrite(buffer,1,data_size,outFile);
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            if(swr_ctx!=NULL){
                swr_free(&swr_ctx);
                swr_ctx=NULL;
            }
        }else{
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }

    fclose(outFile);
    return ;
}
