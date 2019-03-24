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

    CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj);
    ~CallbackUtil();
    void onCallInited(int threadType);
    void onCallLoad(int threadType, bool load);
    void onCallTimeUpdate(int threadType,int currentTime,int totalTime);


};
#endif //AUDIOCOURSE_CALLBACKUTIL_H
