//
// Created by zw on 2019/3/13.
//

#ifndef AUDIOCOURSE_PLAYSTATUSUTIL_H
#define AUDIOCOURSE_PLAYSTATUSUTIL_H
class PlayStatusUtil{
public:
    bool status= true;

    bool getStatus();
    void setStatus(bool status);

    PlayStatusUtil();
    ~PlayStatusUtil();

};
#endif //AUDIOCOURSE_PLAYSTATUSUTIL_H
