#include <jni.h>
#include <string>
#include "src/log.h"
//#include "FfmepgPlayer.h"
#include "src/FfmepgPlayer.h"
#include "src/CallbackUtil.h"

FfmpegPlayer *ffmpegPlayer=NULL;
PlayStatusUtil *playStatus=NULL;
CallbackUtil *callBack=NULL;
pthread_t thread_start;
_JavaVM *javaVM = NULL;
bool nexit= true;
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

//const char *audioPath;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1init(JNIEnv *env, jobject instance,jstring path_) {
     const char *audioPath=env->GetStringUTFChars(path_,0);
    ALOGD("zw:audiopath_%s",audioPath);

    if(ffmpegPlayer==NULL){
        if(callBack==NULL){
            callBack=new CallbackUtil(javaVM,env,&instance);
        }
        playStatus=new PlayStatusUtil();
        ffmpegPlayer=new FfmpegPlayer(playStatus,callBack);
        ffmpegPlayer->init(audioPath);
        env->ReleaseStringUTFChars(path_, audioPath);
    }
}


void *startCallBack(void *data){
    FfmpegPlayer *ffmpegPlayer=(FfmpegPlayer*)data;
    ffmpegPlayer->start();
    return 0;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1start(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        //ffmpegPlayer->startDecode();
        pthread_create(&thread_start,NULL,startCallBack,ffmpegPlayer);
        //ffmpegPlayer->start();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1pause(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->pause();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1resume(JNIEnv *env, jobject instance) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->resume();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1stop(JNIEnv *env, jobject instance) {
    jclass clz=env->GetObjectClass(instance);
    jmethodID jmid_next=env->GetMethodID(clz,"onCallSwitch","()V");

    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->release();
        pthread_join(thread_start,NULL);
        delete(ffmpegPlayer);
        ffmpegPlayer=NULL;
    }
    //指针的new对象都需要delete
    if(callBack!=NULL){
        delete(callBack);
        callBack=NULL;
    }
    if(playStatus!=NULL){
        delete(playStatus);
        playStatus=NULL;
    }
    env->CallVoidMethod(instance,jmid_next);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1seek(JNIEnv *env, jobject instance,
                                                        jint time_sec) {
    if(ffmpegPlayer!=NULL){
        ffmpegPlayer->seek(time_sec);
    }

}