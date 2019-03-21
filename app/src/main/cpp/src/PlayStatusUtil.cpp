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
