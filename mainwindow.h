#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "ARMedia.h"
#include "arvideoplayer.h"
#include "arvideoplaywidget.h"
#include "arvlcplayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    ARVideoPlayer *player;
    ARVlcPlayer *vPlayer;
    ARVideoPlayWidget *playWidget, *playWidget1;
    QList<ARVideoPlayWidget *> playWidgetList;
};

namespace TestSpace {
    class TestClass
    {
    public:
        static void testFunc() { qDebug()<<"test func";}
    };
}
#endif // MAINWINDOW_H
