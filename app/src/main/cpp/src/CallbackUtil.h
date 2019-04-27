//
// Created by zw on 2019/3/13.
//

#ifndef AUDIOCOURSE_CALLBACKUTIL_H
#define AUDIOCOURSE_CALLBACKUTIL_H

#include <jni.h>
#include "log.h"
#define MAIN_THREAD 0
#define CHILD_THREAD 1
class CallbackUtil{
public:
    _JavaVM *javaVM=NULL;
    JNIEnv *jniEnv=NULL;
    jobject jobj;


    jmethodID jmid_inited;
    jmethodID jmid_load;
    jmethodID jmid_timeUpdate;
    jmethodID jmid_renderYUV;
    jmethodID jmid_supportVideo;
    jmethodID jmid_initMediaCodec;
    jmethodID jmid_decodeAVPacket;

    CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj);
    ~CallbackUtil();
    void onCallInited(int threadType);
    void onCallLoad(int threadType, bool load);
    void onCallTimeUpdate(int threadType,int currentTime,int totalTime);
    void onCallRenderYUV(int width,int height,uint8_t *fy,uint8_t *fu,uint8_t *fv);
    bool onCallSupportVideo(const char *ffcodecName);
    void onCallInitMediaCodec(const char *mime,int width,int height,int csd0_size,int csd1_size,uint8_t *csd_0,uint8_t *csd_1);
    void onCallDecodeAVPacket(int dataSize,uint8_t *data);
};
#endif //AUDIOCOURSE_CALLBACKUTIL_H
