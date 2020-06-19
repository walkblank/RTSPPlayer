#ifndef ARVLCPLAYER_H
#define ARVLCPLAYER_H

#include "vlc/vlc.h"
#include "videoplayer.h"

class ARVlcPlayer : public VideoPlayer
{
public:
    ARVlcPlayer(QObject *parent = nullptr);

    int  openPlayer();
    int  closePlayer();
    void readFrames();

private:
    libvlc_instance_t *_vlc;
    libvlc_media_t *_media;
    libvlc_media_player_t *_player;
};

#endif // ARVLCPLAYER_H
