#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include "ARMedia.h"
#include "arframebuf.h"

class VideoPlayer : public QThread
{
    Q_OBJECT
public:
    VideoPlayer(QObject *parent = nullptr)
        :QThread(parent) { setFrameCache(DEFAULT_FRAME_CACHENUM);}
    ~VideoPlayer() {}

    void setMedia(ARMedia media) {this->media = media;}
    void run();

    int quit = 0;
    void setFrameCache(int depth) { frameBuf.setCache(depth); }
    int push_frame(ARFrame *frame) { return frameBuf.push(frame);}
    int pop_frame(ARFrame *frame) { return frameBuf.pop(frame);}

    virtual void readFrames() = 0;
    virtual int openPlayer() = 0;
    virtual int closePlayer() = 0;

    ARFrameBuf frameBuf;

protected:
    ARMedia media;
};
#endif // VIDEOPLAYER_H
