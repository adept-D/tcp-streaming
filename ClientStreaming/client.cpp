#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent},
      camera(nullptr),
      audio(nullptr),
      screen(nullptr),
      buffer(0),
      len(0),
      totalBytes(0)
{
}

void Client::connectServer()
{
    _socket = new QTcpSocket;
    _socket->connectToHost("127.0.0.1",8000);

    connect(_socket, &QTcpSocket::disconnected, _socket, &QTcpSocket::deleteLater);

}

void Client::setCamera(Camera* camera)
{
    this->camera = camera;

    try{
        if(camera == nullptr)throw -1;
    }
    catch(int){
        std::cerr << "Camera is Null" << std::endl;
    }

    connect(camera, &Camera::giveMatFromCamera, this, &Client::getFrame);
    connect(this, &Client::gettedFrame, this, &Client::sendData);
    connect(camera, &Camera::destroyed, camera, &Camera::deleteLater);

}

void Client::setAudio(Audio* audio)
{
    this->audio = audio;

    try{
        if(audio == nullptr)throw -1;
    }
    catch(int){
        std::cerr << "Audio is Null" << std::endl;
    }

    connect(audio, &Audio::giveData, this, &Client::getAudioData);
    connect(this, &Client::gettedAudio, this, &Client::sendData);
    connect(audio, &Audio::destroyed, audio, &Audio::deleteLater);


}

void Client::setScreen(Screen* screen)
{
    this->screen = screen;
    try{
        if(screen == nullptr)throw -1;
    }
    catch(int){
        std::cerr << "Screen is Null" << std::endl;
    }

    connect(screen, &Screen::sendQImage, this, &Client::getScreen);
    connect(this, &Client::gettedScreen, this, &Client::sendData);
    connect(screen, &Screen::destroyed, screen, &Screen::deleteLater);
}

Client::~Client()
{

}


void Client::sendData(const QString& type)
{
//    mutex.lock();
    cv::Mat frame;

    if(type == "cam"){
        frame = mat;
    }
    else if(type == "screen"){
        cv::Mat frameScreen(screenImg.height(),screenImg.width(),CV_8UC4,(void *)screenImg.constBits(),screenImg.bytesPerLine());
        frame = frameScreen;
    }

    quint32 sizeInBytes = frame.size().area() * frame.channels();

    QByteArray frameInByte = QByteArray(reinterpret_cast<char*>(frame.data), sizeInBytes);

    QByteArray allBuf;
    QDataStream out(&allBuf, QIODevice::WriteOnly);
    out << (quint32)0 << (quint32)0 << (quint32)0 << (quint32)0 <<  (quint32)0 << frameInByte  <<  buffer << type; //<< screenInByte; //<< (char*)outdata;

    out.device()->seek(0);
    totalBytes = allBuf.size();

    out << quint32(totalBytes);
    out << quint32((allBuf.size() - sizeof(quint32) * 5)) ;
    out << quint32(frame.rows);
    out << quint32(frame.cols);
    out << quint32(len);

   if(_socket->isOpen() && _socket->state() == QAbstractSocket::ConnectedState){
     _socket->write(allBuf);
     _socket->flush();
   }
   this->buffer.clear();
   this->len = 0;
//   mutex.unlock();
}

void Client::getFrame(const cv::Mat& frame)
{
    this->mat = frame;
    emit gettedFrame("cam");
}

void Client::getScreen(const QImage& screen)
{
    this->screenImg = screen;
    emit gettedScreen("screen");
}



void Client::getAudioData(const QByteArray &buffer, qint64 len)
{
    this->buffer = buffer;
    this->len = len;
    emit gettedAudio("audio");
}
