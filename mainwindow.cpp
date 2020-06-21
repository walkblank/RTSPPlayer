#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

using namespace TestSpace;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("ARVideoPlayer");
    QStringList mediaSrcList;
    mediaSrcList << "rtsp://192.168.1.105/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.163/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.74/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.148/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.92/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.135/h265/ch01/sub/av_stream"
                 << "rtsp://192.168.199.177/h265/ch01/sub/av_stream"
                 << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"
                 << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov"
                 << "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";

    ARMedia media;
    media.type =  MEDIA_TYPE_NETWORK;
    media.resource.path = mediaSrcList[7].toStdString();

    player = new ARVideoPlayer();
    player->setMedia(media);

    vPlayer = new ARVlcPlayer();
    vPlayer->setMedia(media);

    for(int i = 0; i < 16; i ++)
    {
        int row = i / 4;
        int col = i % 4;
        media.resource.path = mediaSrcList[7].toStdString();
        ARVideoPlayWidget *vwidget = new ARVideoPlayWidget();
        vwidget->setMedia(media);
        ui->videoLayout->addWidget(vwidget, row, col);
        playWidgetList << vwidget;
    }
}

MainWindow::~MainWindow()
{
    player->terminate();
    delete player;
    foreach(ARVideoPlayWidget *vwidget, playWidgetList)
    {
        vwidget->stopPlay();
        delete vwidget;
    }
    delete ui;
}


void MainWindow::on_startBtn_clicked()
{
//    player->openPlayer();
//    playWidget->startPlay();
    foreach(ARVideoPlayWidget *vwidget, playWidgetList)
        vwidget->startPlay();
}

void MainWindow::on_stopBtn_clicked()
{
    foreach(ARVideoPlayWidget *vwidget, playWidgetList)
        vwidget->stopPlay();
}

void MainWindow::on_pushButton_clicked()
{
   vPlayer->openPlayer();
}
