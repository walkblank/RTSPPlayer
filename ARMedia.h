#ifndef ARMEDIA_H
#define ARMEDIA_H

#include <string>
#include <list>

typedef enum {
    MEDIA_TYPE_FILE = 0,
    MEDIA_TYPE_NETWORK, // rtsp and so on
    MEDIA_TYPE_CAPTURE,
    MEDIA_TYPE_NB
} media_type_e;

typedef struct ARNetMediaRes {
    std::string ip;
    std::string path,path1;
    std::list<std::string> pathList;
}_ARNetMediaRes;

typedef struct ARMedia {
    media_type_e type;
    ARNetMediaRes resource;
} _ARMedia;

#endif // ARMEDIA_H
