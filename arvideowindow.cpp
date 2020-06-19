#include "arvideowindow.h"

#include <QPainter>
#include <QDebug>

ARVideoWindow::ARVideoWindow(QWidget *parent)
    : QOpenGLWidget(parent)
{

}

ARVideoWindow::~ARVideoWindow()
{

}

void ARVideoWindow::paintEvent(QPaintEvent *evt)
{
    if(!lastFrame.isNull())
    {
        QPainter painter;
        painter.begin(this);
        QImage tempImage((uchar*)lastFrame.buf.base, lastFrame.w, lastFrame.h, QImage::Format_RGB888);
        painter.drawImage(0, 0, tempImage, 0, 0, width(), height());
        painter.end();
    }
}
