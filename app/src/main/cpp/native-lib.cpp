#include <jni.h>
#include <string>
#include "src/log.h"
//#include "FfmepgPlayer.h"
#include "src/FfmepgPlayer.h"
#include "src/CallbackUtil.h"

FfmpegPlayer *ffmpegPlayer=NULL;
PlayStatusUtil *playStatus=NULL;
CallbackUtil *callBack=NULL;

_JavaVM *javaVM = NULL;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    jint result = -1;
    javaVM = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
    {
        return result;
    }
    return JNI_VERSION_1_6;
}
pthread_t initThread;
const char *audioPath;
void *initFun(void *data){
    ffmpegPlayer->init(audioPath);
    pthread_exit(&initThread);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1init(JNIEnv *env, jobject instance,jstring path_) {
     audioPath=env->GetStringUTFChars(path_,0);
    ALOGD("zw:audiopath_%s",audioPath);

    if(ffmpegPlayer==NULL){
        if(callBack==NULL){
            callBack=new CallbackUtil(javaVM,env,&instance);
        }
        playStatus=new PlayStatusUtil();
        ffmpegPlayer=new FfmpegPlayer(playStatus,callBack);
    }
    //pthread_create(&initThread,NULL,initFun,(void*)NULL);
    ffmpegPlayer->init(audioPath);
    env->ReleaseStringUTFChars(path_, audioPath);
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1start(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->startDecode();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1pause(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->pause();
    }
    // TODO

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1resume(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->resume();
    }
    // TODO

}