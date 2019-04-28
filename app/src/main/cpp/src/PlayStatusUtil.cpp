//
// Created by zw on 2019/3/13.
//

#include "PlayStatusUtil.h"

PlayStatusUtil::PlayStatusUtil() {}

PlayStatusUtil::~PlayStatusUtil() {

}

bool PlayStatusUtil::getPlayStatus() {
    return playStatus;
}

void PlayStatusUtil::setPlayStatus(bool status) {
    PlayStatusUtil::playStatus = status;
}

bool PlayStatusUtil::getLoadStatus(){
    return loadStatus;
}

void PlayStatusUtil::setLoadStatus(bool loadStatus) {
    PlayStatusUtil::loadStatus = loadStatus;
}

bool PlayStatusUtil::getSeekStatus() const {
    return seekStatus;
}

void PlayStatusUtil::setSeekStatus(bool seekStatus) {
    PlayStatusUtil::seekStatus = seekStatus;
}

bool PlayStatusUtil::getPauseStatus() const {
    return pauseStatus;
}

void PlayStatusUtil::setPauseStatus(bool pauseStatus) {
    PlayStatusUtil::pauseStatus = pauseStatus;
}
