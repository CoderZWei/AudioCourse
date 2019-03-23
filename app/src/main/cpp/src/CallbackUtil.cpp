//
// Created by zw on 2019/3/13.
//

#include "CallbackUtil.h"

CallbackUtil::CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj) {
    this->javaVM = javaVM;
    this->jniEnv = env;
    this->jobj = *obj;
    this->jobj = env->NewGlobalRef(jobj);

    jclass  jlz = jniEnv->GetObjectClass(jobj);
    if(!jlz)
    {
        ALOGD("get jclass wrong");
        return;
    }

    this->jmid_inited = env->GetMethodID(jlz, "onCallInit", "()V");


    ALOGD("zw_jmid:%d",jmid_inited);
}

CallbackUtil::~CallbackUtil() {

}

void CallbackUtil::onCallInited(int threadType) {
    switch (threadType){
        //主线程里调用回调
        case MAIN_THREAD:
            if(jniEnv==NULL){
                ALOGD("zw:jnienv is null");
            } else{
                ALOGD("zw:jnienv is not null");
            }
            ALOGD("zw:here0");
            this->jniEnv->CallVoidMethod(this->jobj,this->jmid_inited);
            ALOGD("zw:here");
            break;
            //子线程里调用回调
        case CHILD_THREAD:
            JNIEnv *jniEnv;
            if(this->javaVM->AttachCurrentThread(&jniEnv,0)!=JNI_OK){
                ALOGE("zw:get child thread JNIEnv errored");
                return;
            }
            this->jniEnv->CallVoidMethod(this->jobj,this->jmid_inited);
            this->javaVM->DetachCurrentThread();
            break;
    }

}
