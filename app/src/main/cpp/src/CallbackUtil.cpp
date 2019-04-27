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
    this->jmid_timeUpdate=env->GetMethodID(jlz,"onCallTimeUpdate","(II)V");
    this->jmid_renderYUV=env->GetMethodID(jlz,"onCallRenderYUV","(II[B[B[B)V");
    this->jmid_supportVideo=env->GetMethodID(jlz,"onCallSupportMediaCodec","(Ljava/lang/String;)Z");
    this->jmid_initMediaCodec = env->GetMethodID(jlz, "onCallInitMediaCodec", "(Ljava/lang/String;II[B[B)V");
    this->jmid_decodeAVPacket = env->GetMethodID(jlz, "onCallDecodeAVPacket", "(I[B)V");
    //ALOGD("zw_jmid:%d",jmid_inited);
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

void CallbackUtil::onCallLoad(int threadType, bool load) {
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

void CallbackUtil::onCallTimeUpdate(int threadType, int currentTime, int totalTime) {
    if(threadType == MAIN_THREAD)
    {
        jniEnv->CallVoidMethod(jobj, jmid_timeUpdate, currentTime,totalTime);
    }
    else if(threadType == CHILD_THREAD)
    {
        JNIEnv *jniEnv;
        if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
        {
            ALOGD("call onCllLoad worng");
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_timeUpdate,currentTime,totalTime );
        javaVM->DetachCurrentThread();
    }

}

void CallbackUtil::onCallRenderYUV(int width, int height, uint8_t *fy, uint8_t *fu, uint8_t *fv) {
    JNIEnv *jniEnv;
    if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
    {
        ALOGD("call onCallRenderYUV worng");
        return;
    }
    jbyteArray y=jniEnv->NewByteArray(width*height);
    jniEnv->SetByteArrayRegion(y,0,width*height, reinterpret_cast<const jbyte *>(fy));
    jbyteArray u=jniEnv->NewByteArray(width*height/4);
    jniEnv->SetByteArrayRegion(u,0,width*height/4, reinterpret_cast<const jbyte *>(fu));
    jbyteArray v=jniEnv->NewByteArray(width*height/4);
    jniEnv->SetByteArrayRegion(v,0,width*height/4, reinterpret_cast<const jbyte *>(fv));

    jniEnv->CallVoidMethod(jobj,jmid_renderYUV,width,height,y,u,v);

    jniEnv->DeleteLocalRef(y);
    jniEnv->DeleteLocalRef(u);
    jniEnv->DeleteLocalRef(v);
    javaVM->DetachCurrentThread();
}

bool CallbackUtil::onCallSupportVideo(const char *ffcodecName) {

    bool supportVideo= false;
    /*JNIEnv *jniEnv;
    if(javaVM->AttachCurrentThread(&jniEnv,0)!=JNI_OK){
        ALOGD("call onCallSupportVideo worng");
        return supportVideo;
    }*/
    jstring type=jniEnv->NewStringUTF(ffcodecName);
    //ALOGD("zw_code here:%d",supportVideo);
    supportVideo=jniEnv->CallBooleanMethod(jobj,jmid_supportVideo,type);
    //ALOGD("zw_code here1:%d",supportVideo);
    jniEnv->DeleteLocalRef(type);
    //ALOGD("zw_code here2");
    //javaVM->DetachCurrentThread();
    //ALOGD("zw_code here2");
    return supportVideo;
}

void CallbackUtil::onCallInitMediaCodec(const char *mime, int width, int height, int csd0_size,
                                        int csd1_size, uint8_t *csd_0, uint8_t *csd_1) {
    //JNIEnv *jniEnv;
    //if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
    //{
     //       ALOGD("call onCallInitMediaCodec worng");
    //}
    jstring type = jniEnv->NewStringUTF(mime);
    jbyteArray csd0 = jniEnv->NewByteArray(csd0_size);
    jniEnv->SetByteArrayRegion(csd0, 0, csd0_size, reinterpret_cast<const jbyte *>(csd_0));
    jbyteArray csd1 = jniEnv->NewByteArray(csd1_size);
    jniEnv->SetByteArrayRegion(csd1, 0, csd1_size, reinterpret_cast<const jbyte *>(csd_1));

    jniEnv->CallVoidMethod(jobj, jmid_initMediaCodec, type, width, height, csd0, csd1);

    jniEnv->DeleteLocalRef(csd0);
    jniEnv->DeleteLocalRef(csd1);
    jniEnv->DeleteLocalRef(type);
    //javaVM->DetachCurrentThread();
}

void CallbackUtil::onCallDecodeAVPacket(int dataSize, uint8_t *packetData) {
    JNIEnv *jniEnv;
    if(javaVM->AttachCurrentThread(&jniEnv, 0) != JNI_OK)
    {
            ALOGD("call onCallDecodeAVPacket worng");
    }
    jbyteArray data = jniEnv->NewByteArray(dataSize);
    jniEnv->SetByteArrayRegion(data, 0, dataSize, reinterpret_cast<const jbyte *>(packetData));
    jniEnv->CallVoidMethod(jobj, jmid_decodeAVPacket, dataSize, data);
    jniEnv->DeleteLocalRef(data);
    javaVM->DetachCurrentThread();
}


