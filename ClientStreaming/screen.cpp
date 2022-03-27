#include "screen.h"

Screen::Screen(QObject *parent)
    : QObject{parent}
{

}

void Screen::updateFrame()
{
    QDesktopWidget* desktop = QApplication::desktop();
    QScreen* screen =  QGuiApplication::screens().at(0);
    QPixmap screenGrab = screen->grabWindow(desktop->winId());
    QImage image = screenGrab.toImage();

    emit sendQImage(image);
}

bool Screen::isStart()
{
    return this->isStarted;
}

void Screen::stop()
{
    this->isStarted = false;

    emit finished();
}

void Screen::start()
{
    this->isStarted = true;
}
