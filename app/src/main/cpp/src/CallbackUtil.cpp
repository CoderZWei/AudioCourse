//
// Created by zw on 2019/3/13.
//

#include "CallbackUtil.h"

CallbackUtil::CallbackUtil(_JavaVM *javaVM1,JNIEnv *env,jobject *obj) {
    this->javaVM=javaVM1;
    this->jniEnv=env;
    this->jobj=*obj;
    this->jobj=env->GetObjectClass(jobj);
    jclass jCls=this->jniEnv->GetObjectClass(this->jobj);
    this->jmeth_id=jniEnv->GetMethodID(jCls,"onCallback","()V");
}

CallbackUtil::~CallbackUtil() {

}

void CallbackUtil::startCallback(int threadType) {
    switch (threadType){
        //主线程里调用回调
        case MAIN_THREAD:
            this->jniEnv->CallVoidMethod(this->jobj,this->jmeth_id);
            break;
        //子线程里调用回调
        case CHILD_THREAD:
            JNIEnv *jniEnv;
            if(this->javaVM->AttachCurrentThread(&this->jniEnv,0)!=JNI_OK){
                ALOGE("zw:get child thread JNIEnv errored");
                return;
            }
            this->jniEnv->CallVoidMethod(this->jobj,this->jmeth_id);
            this->javaVM->DetachCurrentThread();
            break;
    }

}
