//
// Created by zw on 2019/3/12.
//

#ifndef AUDIOCOURSE_LOG_H
#define AUDIOCOURSE_LOG_H

#include <android/log.h>

static const char *TAG="zetest";
#ifndef ALOGD
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#endif

#ifndef ALOGE
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#endif //AUDIOCOURSE_LOG_H

#endif