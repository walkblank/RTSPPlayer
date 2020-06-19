#ifndef ARVIDEOWINDOW_H
#define ARVIDEOWINDOW_H

#include <QOpenGLWidget>

#include "arframebuf.h"

class ARVideoWindow : public QOpenGLWidget
{
    Q_OBJECT
public:
    ARVideoWindow(QWidget *parent = nullptr);
    ~ARVideoWindow();

    void paintEvent(QPaintEvent *evt);
    ARFrame lastFrame;
};

#endif // ARVIDEOWINDOW_H
