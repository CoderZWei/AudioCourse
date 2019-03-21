//
// Created by zw on 2019/3/13.
//

#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {

}

AudioPlayer::~AudioPlayer() {

}

AudioPlayer::AudioPlayer(PlayStatusUtil *pUtil) {
    this->playStatus=pUtil;
    this->audioQueue=new QueueUtil(playStatus);
}
