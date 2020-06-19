#include "arvlcplayer.h"

#include <mutex>

ARVlcPlayer::ARVlcPlayer(QObject *parent)
    :VideoPlayer(parent)
{
    const char *args[] = {
            "--network-caching=150",
            "--verbose=2" };
    _vlc = libvlc_new(sizeof(args)/sizeof(args[0]),  args);
    _player = libvlc_media_player_new(_vlc);
}

int ARVlcPlayer::openPlayer()
{
    _media = libvlc_media_new_location(_vlc, media.resource.path.c_str());
    libvlc_media_player_set_media(_player, _media);
    libvlc_media_release(_media);

    libvlc_media_player_play(_player);
}

int ARVlcPlayer::closePlayer()
{

}

void ARVlcPlayer::readFrames()
{

}
