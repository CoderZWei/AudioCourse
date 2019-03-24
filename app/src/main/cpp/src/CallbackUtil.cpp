//
// Created by zw on 2019/3/13.
//

#include <pthread.h>
#include "CallbackUtil.h"

CallbackUtil::CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj) {
    this->javaVM = javaVM1;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);

    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if(!jlz)
    {
        return;
    }

    this->jmid_inited = env->GetMethodID(jlz, "onCallInit", "()V");
    this->jmid_load=env->GetMethodID(jlz,"onCallLoad","(Z)V");


    ALOGD("zw_jmid:%d",jmid_inited);
}

CallbackUtil::~CallbackUtil() {

}

void CallbackUtil::onCallInited(int threadType) {
    if(threadType == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_inited);
    }
    else if(threadType == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
           ALOGD("get child thread jnienv worng");

            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_inited);
        javaVM->DetachCurrentThread();
    }
}

void CallbackUtil::onCaLoad(int threadType, bool load) {
    if(threadType == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_load, load);
    }
    else if(threadType == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
           ALOGD("call onCllLoad worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_load, load);
        javaVM->DetachCurrentThread();
    }
}


