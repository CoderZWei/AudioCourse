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

    CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj);
    ~CallbackUtil();
    void onCallInited(int threadType);
    void onCaLoad(int threadType, bool load);



};
#endif //AUDIOCOURSE_CALLBACKUTIL_H
