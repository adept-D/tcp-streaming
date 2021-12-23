#include "camera.h"

Camera::Camera(QObject *parent)
    : QObject{parent}
{

}

bool Camera::isStart()
{
    return this->isStarted;
}

void Camera::start(){
    vcap.open(0);
    if(!vcap.isOpened()){
           std::cout << "Error opening video stream or file" << std::endl;
           return;
    }
    this->isStarted = true;
}

void Camera::stop()
{
    this->vcap.release();
    this->isStarted = false;
    emit finished();
}

void Camera::updateFrame()
{
    if(vcap.isOpened()){
        cv::Mat frame;
        vcap >> frame;

        emit giveMatFromCamera(frame);
    }

}
