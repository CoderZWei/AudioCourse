//
// Created by zw on 2019/3/13.
//

#include "PlayStatusUtil.h"

PlayStatusUtil::PlayStatusUtil() {}

PlayStatusUtil::~PlayStatusUtil() {

}

bool PlayStatusUtil::getStatus() {
    return status;
}

void PlayStatusUtil::setStatus(bool status) {
    PlayStatusUtil::status = status;
}

bool PlayStatusUtil::getLoadStatus(){
    return loadStatus;
}

void PlayStatusUtil::setLoadStatus(bool loadStatus) {
    PlayStatusUtil::loadStatus = loadStatus;
}
