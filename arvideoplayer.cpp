#include "arvideoplayer.h"

#include <QDebug>

ARVideoPlayer::ARVideoPlayer(QObject *parent)
    : VideoPlayer(parent)
{
    avdevice_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_QUIET);
}

ARVideoPlayer::~ARVideoPlayer()
{
}

int ARVideoPlayer::closePlayer()
{

}

bool ARVideoPlayer::findAvailableHWCodec(AVCodecParameters *codecParam)
{
    bool beFind = false;

    enum AVHWDeviceType type;
    type = av_hwdevice_find_type_by_name("dxva2");

    qDebug("hw device %s", av_hwdevice_get_type_name(type));
    qDebug()<<type;

    return beFind;
}

int ARVideoPlayer::openPlayer()
{

    AVCodec *codec = nullptr;
    int ret= 0;
    fmt_ctx = avformat_alloc_context();
    av_dict_set(&fmt_opts, "stimeout", "5000000", 0); // 设置打开rtsp流超时时间
    av_dict_set(&fmt_opts, "buffer_size", "2048000", 0);
    ret = avformat_open_input(&fmt_ctx,  media.resource.path.data(), nullptr, &fmt_opts);
    if(ret < 0)
    {
        qDebug()<<"open input failed";
        return -10;
    }
    ret = avformat_find_stream_info(fmt_ctx, nullptr);
    if(ret < 0)
    {
        qDebug()<<"can't get stream info";
        return -20;
    }

    qDebug()<<"stream num" << fmt_ctx->nb_streams;
    video_stream_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &codec, 0);
    qDebug()<< "video stream index" << video_stream_index;

    AVStream *videoStream = fmt_ctx->streams[video_stream_index];
    video_time_base_num = videoStream->time_base.num;
    video_time_base_den = videoStream->time_base.den;
    qDebug("video steam timebase %d %d", video_time_base_num, video_time_base_den);

    AVCodecParameters *codecParam = videoStream->codecpar;
    qDebug("codec param = %d : %s", codecParam->codec_id, avcodec_get_name(codecParam->codec_id));

    std::string decoder(avcodec_get_name(codecParam->codec_id));
    if(codec == nullptr)
    {
        qDebug()<<"find codec";
        codec = avcodec_find_decoder_by_name(decoder.c_str());
    }

    if(codec == nullptr)
    {
        qDebug()<<"can't find decoder";
        return -30;
    }

    codec_ctx = avcodec_alloc_context3(codec);
    if(codec_ctx == nullptr)
    {
        qDebug()<<"alloc codecCtx failed";
        return -40;
    }

    ret = avcodec_parameters_to_context(codec_ctx, codecParam);
    if(codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
        av_dict_set(&codec_opts, "refcounted_frames", "1", 0);

    ret = avcodec_open2(codec_ctx, codec, &codec_opts);
    if(ret != 0)
    {
        qDebug()<<"decoder open failed" << ret;
        return -50;
    }

    videoStream->discard = AVDISCARD_DEFAULT;
    int sw, sh, dw, dh;
    sw = codec_ctx->width;
    sh = codec_ctx->height;
    src_pix_fmt = codec_ctx->pix_fmt;
    qDebug("sw=%d sh=%d src_pix_fmt=%d:%s", sw, sh, src_pix_fmt, av_get_pix_fmt_name(src_pix_fmt));
    if (sw <= 0 || sh <= 0 || src_pix_fmt == AV_PIX_FMT_NONE) {
        qDebug("Codec parameters wrong!");
        ret = -45;
        return ret;
    }

    //    dw = sw >> 2 << 2; // align = 4
    dw = 640;// align = 4
    //    dh = sh;
    dh = 320;

    dst_pix_fmt = AV_PIX_FMT_RGB24;
    qDebug("dw=%d dh=%d dst_pix_fmt=%d:%s", dw, dh, dst_pix_fmt, av_get_pix_fmt_name(dst_pix_fmt));
    sws_ctx = sws_getContext(sw, sh, src_pix_fmt, dw, dh, dst_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
    if (sws_ctx == NULL) {
        qDebug("sws_getContext");
        ret = -50;
        return ret;
    }

    packet = av_packet_alloc();
    frame = av_frame_alloc();

    arframe.w = dw;
    arframe.h = dh;
    // ARGB
    arframe.buf.resize(dw * dh * 4);

    //    arframe.type = PIX_FMT_BGR;
    arframe.bpp = 24;
    arframe.buf.len = dw * dh * 3;
    data[0] = (uint8_t*)arframe.buf.base;
    linesize[0] = dw * 3;

    if (videoStream->avg_frame_rate.num && videoStream->avg_frame_rate.den) {
        fps = videoStream->avg_frame_rate.num / videoStream->avg_frame_rate.den;
    }


    width = sw;
    height = sh;
    duration = 0;
    start_time = 0;
    eof = 0;
    error = 0;
    if (video_time_base_num && video_time_base_den) {
        if (videoStream->duration > 0) {
            duration = videoStream->duration / (double)video_time_base_den * video_time_base_num * 1000;
        }
        if (videoStream->start_time > 0) {
            start_time = videoStream->start_time / (double)video_time_base_den * video_time_base_num * 1000;
        }
    }
    qDebug("fps=%d duration=%lldms start_time=%lldms", fps, duration, start_time);

    return ret;
}

void ARVideoPlayer::readFrames()
{
    int ret = 0;
    while(!quit)
    {
        av_init_packet(packet);
        ret = av_read_frame(fmt_ctx, packet);
        if(ret != 0)
        {
            qDebug()<<"read frame failed";
        }

        // only read video frame
        if(packet->stream_index != video_stream_index)
            continue;

        ret = avcodec_send_packet(codec_ctx, packet);
        if(ret != 0)
        {
            qDebug()<< "send packet failed";
            return;
        }

        ret = avcodec_receive_frame(codec_ctx, frame);
        if(ret != 0)
        {
            qDebug()<<"received frame failed";
            return;
        }
        else
            break;
        msleep(15);
    }

    if(sws_ctx)
    {
//        qDebug("sws_scale w=%d h=%d data=%p", frame->width, frame->height, frame->data);
        int h =  sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, data, linesize);
        if(h <= 0)
        {
//            qDebug("sws_scale h=%d", h);
            return;
        }
    }

    if (video_time_base_num && video_time_base_den) {
        arframe.ts = frame->pts / (double)video_time_base_den * video_time_base_num * 1000;
    }

    push_frame(&arframe);
}
