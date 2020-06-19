#ifndef ARFRAMEBUF_H
#define ARFRAMEBUF_H

#include <stdint.h>
#include <queue>
#include <mutex>

extern "C"
{
#include "buf.h"
}

class ARFrame {
public:
    ARBuf buf;
    int w;
    int h;
    int bpp;
    int type;
    uint64_t ts;
    int64_t useridx;
    void* userdata;

    ARFrame() {
        w = h = bpp = type = 0;
        ts = 0;
        useridx = -1;
        userdata = NULL;
    }

    bool isNull() {
        return w == 0 || h == 0 || buf.isNull();
    }

    void copy(const ARFrame& rhs) {
        w = rhs.w;
        h = rhs.h;
        bpp = rhs.bpp;
        type = rhs.type;
        ts = rhs.ts;
        useridx = rhs.useridx;
        userdata = rhs.userdata;
        buf.copy(rhs.buf.base, rhs.buf.len);
    }
};

typedef struct frame_info_s {
    int w;
    int h;
    int type;
    int bpp;
} FrameInfo;

typedef struct frame_stats_s {
    int push_cnt;
    int pop_cnt;

    int push_ok_cnt;
    int pop_ok_cnt;

    frame_stats_s() {
        push_cnt = pop_cnt = push_ok_cnt = pop_ok_cnt = 0;
    }
} FrameStats;

#define DEFAULT_FRAME_CACHENUM  10

class ARFrameBuf : public ARRingBuf {
 public:
    enum CacheFullPolicy {
        SQUEEZE,
        DISCARD,
    } policy;

    ARFrameBuf() : ARRingBuf() {
        cache_num = DEFAULT_FRAME_CACHENUM;
        policy = SQUEEZE;
    }

    void setCache(int num) {cache_num = num;}
    void setPolicy(CacheFullPolicy policy) {this->policy = policy;}

    int push(ARFrame* pFrame);
    int pop(ARFrame* pFrame);

    int         cache_num;
    FrameStats  frame_stats;
    FrameInfo   frame_info;
    std::deque<ARFrame> frames;
    std::mutex         mutex;
};

#endif // ARFRAMEBUF_H
