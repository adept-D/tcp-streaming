#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QScreen>
#include <QDesktopWidget>
#include <QApplication>
#include <QGuiApplication>
#include <QImage>

class Screen : public QObject
{
    Q_OBJECT
public:
    explicit Screen(QObject *parent = nullptr);
    void updateFrame();
    bool isStart();
    void stop();
    void start();
private:
    bool isStarted = false;

signals:
    void sendQImage(const QImage&);
    void finished();
};

#endif // SCREEN_H
