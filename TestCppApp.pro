QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#ffmpeg libs
INCLUDEPATH += ../3rd/ffmpeg-dev/ffmpeg-4.2.3-win32-dev/include
LIBS += -L../3rd/ffmpeg-dev/ffmpeg-4.2.3-win32-dev/lib
LIBS += -L../3rd/ffmpeg-dev/ffmpeg-4.2.3-win32-shared/bin
LIBS += -lavformat -lavcodec -lavutil -lswscale -lavdevice

#libvlc
INCLUDEPATH += ../3rd/vlc-3.0.8/sdk/include
LIBS += -L../3rd/vlc-3.0.8/lib
LIBS += -L../3rd/vlc-3.0.8
LIBS += -llibvlc -llibvlccore

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arframebuf.cpp \
    arvideoplayer.cpp \
    arvideoplaywidget.cpp \
    arvideowindow.cpp \
    arvlcplayer.cpp \
    base.c \
    main.cpp \
    mainwindow.cpp \
    videoplayer.cpp

HEADERS += \
    ARMedia.h \
    arframebuf.h \
    arvideoplayer.h \
    arvideoplaywidget.h \
    arvideowindow.h \
    arvlcplayer.h \
    base.h \
    buf.h \
    mainwindow.h \
    videoplayer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
