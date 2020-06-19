#include "videoplayer.h"

void VideoPlayer::run()
{
    if(openPlayer() < 0)
        return;
    while(!quit)
    {
        readFrames();
        msleep(15);
    }
}
