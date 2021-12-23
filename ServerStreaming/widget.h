#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "server.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QByteArray>
#include <QTimer>
#include "opencv2/opencv.hpp"
#include <QBuffer>
#include <QGridLayout>
#include <QThread>
#include <QPixmap>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include "image.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QLabel* img_label;
    Image* image;
    Server* server;
    QGridLayout* grid;
    QImage frameImg;

    QIODevice* output;          //
    QAudioOutput* audioOutput;  //  Работа с аудио
    QAudioFormat format;        //
    QAudioDeviceInfo toOutput;  //
private:
    void initAudioFormat();
private slots:
    void setImageFromByte(QByteArray, int, int, int);
    void setAudio(QByteArray, quint32);

};
#endif // WIDGET_H
