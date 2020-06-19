#ifndef BASEBUF_H
#define BASEBUF_H


#include <stdlib.h>
#include <string.h>
#include "base.h"

typedef struct ar_buf_s {
    char *base;
    size_t len;

#ifdef __cplusplus
    ar_buf_s(){
        this->base = nullptr;
        this->len = 0;
    }
    ar_buf_s(void *data, int len) {
        this->base = (char*)data;
        this->len = len;
    }
#endif
} ar_buf_t;


#ifdef __cplusplus
class ARBuf : public ar_buf_t {
public:
    ARBuf() : ar_buf_t() {
        cleanup_ = false;
    }
    ARBuf(void* data, size_t len) : ar_buf_t(data, len) {
        cleanup_ = false;
    }
    ARBuf(size_t cap) { resize(cap); }

    virtual ~ARBuf() {
        cleanup();
    }

    void*  data() { return base; }
    size_t size() { return len; }

    bool isNull() {
        return base == NULL || len == 0;
    }

    void cleanup() {
        if (cleanup_) {
            SAFE_FREE(base);
            len = 0;
            cleanup_ = false;
        }
    }

    void resize(size_t cap) {
        if (cap == len) return;

        if (base == NULL) {
            SAFE_ALLOC(base, cap);
        }
        else {
            base = (char*)safe_realloc(base, cap, len);
        }
        len = cap;
        cleanup_ = true;
    }

    void copy(void* data, size_t len) {
        resize(len);
        memcpy(base, data, len);
    }

    void copy(ar_buf_t* buf) {
        copy(buf->base, buf->len);
    }

private:
    bool cleanup_;
};

class ARRingBuf : public ARBuf {
public:
    ARRingBuf() : ARBuf() {_head = _tail = _size = 0;}
    ARRingBuf(size_t cap) : ARBuf(cap) {_head = _tail = _size = 0;}
    virtual ~ARRingBuf() {}

    char* alloc(size_t len) {
        char* ret = NULL;
        if (_head < _tail || _size == 0) {
            // [_tail, this->len) && [0, _head)
            if (this->len - _tail >= len) {
                ret = base + _tail;
                _tail += len;
                if (_tail == this->len) _tail = 0;
            }
            else if (_head >= len) {
                ret = base;
                _tail = len;
            }
        }
        else {
            // [_tail, _head)
            if (_head - _tail >= len) {
                ret = base + _tail;
                _tail += len;
            }
        }
        _size += ret ? len : 0;
        return ret;
    }

    void free(size_t len) {
        _size -= len;
        if (len <= this->len - _head) {
            _head += len;
            if (_head == this->len) _head = 0;
        }
        else {
            _head = len;
        }
    }

    size_t size() {return _size;}

private:
    size_t _head;
    size_t _tail;
    size_t _size;
};

#endif

#endif // BASEBUF_H
