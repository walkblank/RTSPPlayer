#include "arvideoplaywidget.h"

#include <QImage>
#include <QVBoxLayout>
#include <QDebug>


ARVideoPlayWidget::ARVideoPlayWidget(QWidget *parent) : QFrame(parent)
{
    initUI();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onVideoWindowUpdateTimer()));
}

ARVideoPlayWidget::~ARVideoPlayWidget()
{
    delete player;
    delete videowind;
}

void ARVideoPlayWidget::initUI()
{
    videowind = new ARVideoWindow(this);
    player = new ARVideoPlayer();
    QVBoxLayout *boxlayout = new QVBoxLayout();
    boxlayout->addWidget(videowind);
    boxlayout->setGeometry(QRect(0,0,640, 360));
}

void ARVideoPlayWidget::onVideoWindowUpdateTimer()
{
    if(player->pop_frame(&videowind->lastFrame) == 0)
    {
        videowind->update();
    }
}

void ARVideoPlayWidget::setMedia(ARMedia m)
{
    this->media = m;
    player->setMedia(m);
}

void ARVideoPlayWidget::startPlay()
{
    player->start();
    timer->start(40);
}

void ARVideoPlayWidget::stopPlay()
{
    timer->stop();
    player->terminate();
}
