#ifndef ARVIDEOPLAYWIDGET_H
#define ARVIDEOPLAYWIDGET_H

#include <QFrame>
#include <QTimer>

#include "ARMedia.h"
#include "arvideoplayer.h"
#include "arvideowindow.h"

class ARVideoPlayWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ARVideoPlayWidget(QWidget *parent = nullptr);
    ~ARVideoPlayWidget();
    void setMedia(ARMedia m);
    void startPlay();
    void stopPlay();

private slots:
    void onVideoWindowUpdateTimer();

private:
    ARMedia media;
    VideoPlayer *player;
    ARVideoWindow *videowind;

    void initUI();
    void initContent();
    QTimer *timer;
};

#endif // ARVIDEOPLAYWIDGET_H
