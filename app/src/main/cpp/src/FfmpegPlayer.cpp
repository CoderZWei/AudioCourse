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
    pthread_mutex_destroy(&init_mutex);
    pthread_mutex_destroy(&seek_mutex);
}
FfmpegPlayer::FfmpegPlayer(PlayStatusUtil *pUtil,CallbackUtil *callbackUtil) {
    this->playStatus=pUtil;
    this->callbackUtil=callbackUtil;
    pthread_mutex_init(&init_mutex,NULL);
    pthread_mutex_init(&seek_mutex,NULL);
    exit= false;
}

void FfmpegPlayer::init(const char *url) {
    pthread_mutex_lock(&init_mutex);
    //audioPlayer=new AudioPlayer(this->playStatus,44100);
    this->url=url;
    ALOGD("zw:url_%s",this->url);
    av_register_all();

    avformat_network_init();

    pFormatCtx=avformat_alloc_context();
    if(avformat_open_input(&pFormatCtx,this->url,NULL,NULL)!=0){
        ALOGE("zw:can't open url:%s",this->url);
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    if(avformat_find_stream_info(pFormatCtx,NULL)<0){
        ALOGE("zw:can't find stream info:%s",this->url);
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return;
    }
    //ALOGD("zw:here");
    for(int i=0;i<pFormatCtx->nb_streams;i++){
        if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_AUDIO) { //codecpar用于记录编码后的信息
                if(audioPlayer==NULL){
                    audioPlayer=new AudioPlayer(this->playStatus,this->callbackUtil,this->pFormatCtx->streams[i]->codecpar->sample_rate);
                    audioPlayer->streamIndex=i;
                    audioPlayer->codecPar=pFormatCtx->streams[i]->codecpar;
                    audioPlayer->duration=pFormatCtx->duration/AV_TIME_BASE;
                    audioPlayer->time_base=pFormatCtx->streams[i]->time_base;//每一帧所持续的时间
                    duration=audioPlayer->duration;
                }
            }
        else if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
            if(videoPlayer==NULL){
                videoPlayer=new VideoPlayer(this->playStatus,this->callbackUtil);
                videoPlayer->streamIndex=i;
                videoPlayer->codecPar=pFormatCtx->streams[i]->codecpar;
                videoPlayer->time_base=pFormatCtx->streams[i]->time_base;

                int num=pFormatCtx->streams[i]->avg_frame_rate.num;
                int den=pFormatCtx->streams[i]->avg_frame_rate.den;
                if(num!=0 && den!=0){
                    int fps=num/den;
                    videoPlayer->defaultDelayTime=1.0/fps;
                }
            }
        }
    }
    if(audioPlayer!=NULL){
        getCodecContext(audioPlayer->codecPar,&audioPlayer->avCodecContext);
    }
    if(videoPlayer!=NULL){
        getCodecContext(videoPlayer->codecPar,&videoPlayer->avCodecContext);
        ALOGD("zw_avCodecContext width:%d height:%d",videoPlayer->avCodecContext->width,videoPlayer->avCodecContext->height);
    }
    if(callbackUtil!=NULL && playStatus->getStatus()== true){
        callbackUtil->onCallInited(MAIN_THREAD);
    } else{
        exit= true;
    }
    pthread_mutex_unlock(&init_mutex);
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
    if(videoPlayer==NULL){
        ALOGE("zw:vidioPlayer is null");
        return;
    }
    audioPlayer->play();
    videoPlayer->audioPlayer=audioPlayer;
    videoPlayer->play();
    int audioCount=0,videoCount=0;
    while (playStatus!=NULL && playStatus->getStatus()== true ){
        if(playStatus->getSeekStatus()== true){//当前是seek状态时，停止解码
            av_usleep(1000 * 100);
            continue;
        }
        if(audioPlayer->audioQueue->getQueueSize()>40){//防止队列中存储太多packet
            av_usleep(1000 * 100);
            continue;
        }
        AVPacket *avPacket=av_packet_alloc();
        if(av_read_frame(pFormatCtx,avPacket)==0){
            if(avPacket->stream_index==audioPlayer->streamIndex){
                audioCount++;
                //ALOGD("zw:解码音频第%d帧",audioCount);
                //ALOGD("zw_音频size:%d",audioPlayer->audioQueue->getQueueSize());
                audioPlayer->audioQueue->putAVPacket(avPacket);
                //av_packet_free(&avPacket);
                //av_free(avPacket);
            } else if(avPacket->stream_index==videoPlayer->streamIndex){
                videoCount++;
                videoPlayer->videoQueue->putAVPacket(avPacket);
                //ALOGD("zw_videoSize0:%d",videoPlayer->videoQueue->getQueueSize());
               // ALOGD("zw:解码视频第%d帧",videoCount);
            }
            else{
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
                    av_usleep(1000 * 100);
                    continue;
                } else{
                    playStatus->setStatus(false);
                    break;
                }
            }
        }
    }
    exit= true;
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
    if(playStatus!=NULL){
        playStatus->setPauseStatus(true);
    }
    if(audioPlayer!=NULL){
        audioPlayer->pause();
    }

}

void FfmpegPlayer::resume() {
    if(audioPlayer!=NULL){
        audioPlayer->resume();
    }
    if(playStatus!=NULL){
        playStatus->setPauseStatus(false);
    }
}

void FfmpegPlayer::release() {
    /*
    if(playStatus->getStatus()== false){
        return;
    }
     */
    playStatus->setStatus(false);
    pthread_mutex_lock(&init_mutex);
    int sleepCount=0;
    while (exit== false){
        if(sleepCount > 1000)
        {
            exit = true;
        }
        sleepCount++;
        av_usleep(1000 * 10);//暂停10毫秒
    }
    if(audioPlayer!=NULL){
        audioPlayer->release();
        delete(audioPlayer);
        audioPlayer=NULL;
    }
    if(videoPlayer!=NULL){
        videoPlayer->release();
        delete(videoPlayer);
        videoPlayer=NULL;
    }
    if(pFormatCtx!=NULL){
        avformat_close_input(&pFormatCtx);
        avformat_free_context(pFormatCtx);
        pFormatCtx=NULL;
    }
    //已经在native-lib中delete
    if(callbackUtil!=NULL){
        callbackUtil=NULL;
    }
    if(playStatus!=NULL){
        playStatus=NULL;
    }
    pthread_mutex_unlock(&init_mutex);
}

void FfmpegPlayer::seek(int64_t time_sec) {
    if(duration<0 || time_sec>duration){
        return;
    } else{
        playStatus->setSeekStatus(true);
        pthread_mutex_lock(&seek_mutex);
        int64_t rel=time_sec*AV_TIME_BASE;
        avformat_seek_file(pFormatCtx, -1, INT64_MIN, rel, INT64_MAX, 0);
        if(audioPlayer!=NULL){
            audioPlayer->audioQueue->clearAVPacket();
            audioPlayer->clock=0;
            audioPlayer->last_time=0;
            pthread_mutex_lock(&audioPlayer->codecMutex);
            avcodec_flush_buffers(audioPlayer->avCodecContext);
            pthread_mutex_unlock(&audioPlayer->codecMutex);
        }
        if(videoPlayer!=NULL){
            videoPlayer->videoQueue->clearAVPacket();
            videoPlayer->clock=0;
            pthread_mutex_lock(&videoPlayer->codecMutex);
            avcodec_flush_buffers(videoPlayer->avCodecContext);
            pthread_mutex_unlock(&videoPlayer->codecMutex);
        }

        pthread_mutex_unlock(&seek_mutex);
        playStatus->setSeekStatus(false);
    }

}

int FfmpegPlayer::getCodecContext(AVCodecParameters *codecpar, AVCodecContext **avCodecContext) {
    AVCodec *dec=avcodec_find_decoder(codecpar->codec_id);
    if(!dec){
        ALOGE("zw:can't find decoder");
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return -1;
    }
    *avCodecContext=avcodec_alloc_context3(dec);
   /* if(!audioPlayer->avCodecContext){
        ALOGE("zw:can't alloc new decodectx");
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return -1;
    }*/
    if(avcodec_parameters_to_context(*avCodecContext,codecpar)<0){
        ALOGE("zw:can't fill decodecctx");
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return -1;
    }
    if(avcodec_open2(*avCodecContext,dec,0)!=0){
        ALOGE("zw:can't open audio sreams");
        exit= true;
        pthread_mutex_unlock(&init_mutex);
        return -1;
    }
    ALOGD("zw:HERE");

    return 0;
}






