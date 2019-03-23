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
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        return result;
    }
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_zw_audiocourse_FfmpegWrapper_cpp_1init(JNIEnv *env, jobject instance,jstring path_) {
    const char *audioPath=env->GetStringUTFChars(path_,0);
    ALOGD("zw:audiopath_%s",audioPath);
    if(callBack==NULL){
        callBack=new CallbackUtil(javaVM,env,&instance);
    }
    if(ffmpegPlayer==NULL){

        playStatus=new PlayStatusUtil();
        ffmpegPlayer=new FfmpegPlayer(playStatus,callBack);
    }
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

