//
// Created by zw on 2019/3/13.
//

#ifndef AUDIOCOURSE_PLAYSTATUSUTIL_H
#define AUDIOCOURSE_PLAYSTATUSUTIL_H
class PlayStatusUtil{
public:
    bool status= true;
    bool loadStatus= true;
    bool seekStatus= false;
    bool pauseStatus= false;

    bool getPauseStatus() const;

    void setPauseStatus(bool pauseStatus);

    bool getSeekStatus() const;

    void setSeekStatus(bool seekStatus);

    bool getLoadStatus();

    void setLoadStatus(bool loadStatus);

    bool getStatus();
    void setStatus(bool status);

    PlayStatusUtil();
    ~PlayStatusUtil();

};
#endif //AUDIOCOURSE_PLAYSTATUSUTIL_H
