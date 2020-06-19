#ifndef ARVIDEOPLAYER_H
#define ARVIDEOPLAYER_H

#define DEFAULT_FRAME_CACHENUM 5

#include <QThread>
#include "ARMedia.h"
#include "videoplayer.h"

extern "C" {
#include "libavutil/avutil.h"
#include "libavutil/log.h"
#include "libavutil/pixdesc.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
}

class ARVideoPlayer  : public VideoPlayer
{

public:
    ARVideoPlayer(QObject *parent  = nullptr);
    ~ARVideoPlayer();

    int openPlayer();
    int closePlayer();
    void readFrames();

private:

    int decode_mode ;
    static std::atomic_flag s_ffmpeg_init;

    AVDictionary*       fmt_opts = nullptr;
    AVDictionary*       codec_opts = nullptr;

    AVFormatContext*    fmt_ctx;
    AVCodecContext*     codec_ctx;

    AVPacket* packet;
    AVFrame* frame;

    int video_stream_index;
    int audio_stream_index;
    int subtitle_stream_index;

    int video_time_base_num;
    int video_time_base_den;

    // for scale
    AVPixelFormat   src_pix_fmt;
    AVPixelFormat   dst_pix_fmt;
    SwsContext*     sws_ctx;
    uint8_t*        data[4];
    int             linesize[4];
    ARFrame         arframe;
//    ARMedia media;

//    ARFrameBuf      frameBuf;
    int fps;


    //statistics
    int32_t     width;
    int32_t     height;
    int64_t     duration;   // ms
    int64_t     start_time; // ms
    int         eof;
    int         error;

    // thread ctrl
//    int quit = 0;

    bool findAvailableHWCodec(AVCodecParameters *codecParam);
};

#endif // ARVIDEOPLAYER_H
