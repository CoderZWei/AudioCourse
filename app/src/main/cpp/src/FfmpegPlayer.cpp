//
// Created by zw on 2019/3/12.
//
//#include <include/libavcodec/avcodec.h>
//#include <include/libavutil/avutil.h>
#include "FfmepgPlayer.h"
#include "log.h"

FfmpegPlayer::FfmpegPlayer() {
}

FfmpegPlayer::~FfmpegPlayer() {
}
FfmpegPlayer::FfmpegPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil) {
    this->playStatus=pUtil;
    this->callbackUtil=callbackUtil;
}

void FfmpegPlayer::init(const char *url) {
    //audioPlayer=new AudioPlayer(this->playStatus,44100);
    this->url=url;
    ALOGD("zw:url_%s",this->url);
    av_register_all();

    avformat_network_init();

    pFormaxCtx=avformat_alloc_context();
    if(avformat_open_input(&pFormaxCtx,this->url,NULL,NULL)!=0){
        ALOGE("zw:can't open url",this->url);
        return;
    }
    if(avformat_find_stream_info(pFormaxCtx,NULL)<0){
        ALOGE("zw:can't find stream info",this->url);
        return;
    }
    ALOGD("zw:here");
    for(int i=0;i<pFormaxCtx->nb_streams;i++){
        if(pFormaxCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO) { //codecpar用于记录编码后的信息
                if(audioPlayer==NULL){
                    audioPlayer=new AudioPlayer(this->playStatus,this->callbackUtil,this->pFormaxCtx->streams[i]->codecpar->sample_rate);
                    audioPlayer->streamIndex=i;
                    audioPlayer->codecPar=pFormaxCtx->streams[i]->codecpar;
                    audioPlayer->duration=pFormaxCtx->duration/AV_TIME_BASE;
                    audioPlayer->time_base=pFormaxCtx->streams[i]->time_base;
                }
            }
    }
    AVCodec *dec=avcodec_find_decoder(audioPlayer->codecPar->codec_id);
    if(!dec){
        ALOGE("zw:can't find decoder");
        return;
    }
    audioPlayer->avCodecContext=avcodec_alloc_context3(dec);
    if(!audioPlayer->avCodecContext){
        ALOGE("zw:can't alloc new decodectx");
        return;
    }
    if(avcodec_parameters_to_context(audioPlayer->avCodecContext,audioPlayer->codecPar)){
        ALOGE("zw:can't fill decodecctx");
        return;
    }
    if(avcodec_open2(audioPlayer->avCodecContext,dec,0)!=0){
        ALOGE("zw:can't open audio sreams");
    }
    ALOGD("zw:HERE");
    callbackUtil->onCallInited(MAIN_THREAD);
}


void *decodeFFmpeg(void *data){
    FfmpegPlayer *ffmpegPlayer=(FfmpegPlayer*)data;
    ffmpegPlayer->startDecode();
    pthread_exit(&ffmpegPlayer->decodeThread);
}

void FfmpegPlayer::startDecode() {
    if(audioPlayer==NULL){
        ALOGE("zw:audioPlayer is null");
        return;
    }
    audioPlayer->play();
    int count=0;
    while (playStatus!=NULL && playStatus->getStatus()== true ){
        AVPacket *avPacket=av_packet_alloc();
        if(av_read_frame(pFormaxCtx,avPacket)==0){
            if(avPacket->stream_index==audioPlayer->streamIndex){
                count++;
                ALOGD("zw:解码第%d帧",count);
                ALOGD("zw_size:%d",audioPlayer->audioQueue->getQueueSize());
                audioPlayer->audioQueue->putAVPacket(avPacket);
                //av_packet_free(&avPacket);
                //av_free(avPacket);
            } else{
                av_packet_free(&avPacket);
                av_free(avPacket);
            }
        } else{
            //ALOGD("zw:解码结束");
            av_packet_free(&avPacket);
            av_free(avPacket);

            while (playStatus!=NULL && playStatus->getStatus()== true){
                //因为队列里可能有缓存，所以不能直接退出，而是要等待
                if(audioPlayer->audioQueue->getQueueSize()>0){
                    continue;
                } else{
                    playStatus->setStatus(false);
                    break;
                }
            }

        }
    }
    /*
    //模拟出队
    while (this->audioPlayer->audioQueue->getQueueSize()>0){
        AVPacket *packet=av_packet_alloc();
        this->audioPlayer->audioQueue->getAVPacket(packet);
        av_packet_free(&packet);
        av_free(packet);
        packet=NULL;
    }
    */
    ALOGD("解码完成");
}

void FfmpegPlayer::start() {
    /*
    第一个参数为指向线程标识符的指针。
　　第二个参数用来设置线程属性。
　　第三个参数是线程运行函数的地址。
　　最后一个参数是运行函数的参数。
     */
    pthread_create(&decodeThread,NULL,decodeFFmpeg,this);
}

void FfmpegPlayer::pause() {
    if(audioPlayer!=NULL){
        audioPlayer->pause();
    }
}

void FfmpegPlayer::resume() {
    if(audioPlayer!=NULL){
        audioPlayer->resume();
    }
}






