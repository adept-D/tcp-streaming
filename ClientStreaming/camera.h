#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "opencv2/opencv.hpp"
#include "audio.h"
#include <iostream>
#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <QThread>

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    bool isStart();
    void start();
    void stop();
    void updateFrame();
private: //variables
    cv:: VideoCapture vcap;
//    bool isSend = false;
    bool isStarted = false;


signals:
    void giveMatFromCamera(const cv::Mat&);
//    void giveVideoCapture(cv::VideoCapture&);
    void finished();
};

#endif // CAMERA_H
