#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

#include "camera.h"
#include "audio.h"
#include "screen.h"

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QMutex>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectServer();

    void setCamera(Camera*);
    void setAudio(Audio*);
    void setScreen(Screen*);
    ~Client();
private: //var
    QTcpSocket* _socket;

    QMutex mutex;

    Camera* camera;
    Audio* audio;
    Screen* screen;

    QByteArray buffer;
    qint64 len;
    quint32 totalBytes;

    QImage screenImg;
    cv:: Mat mat;

private slots:
    void getAudioData(const QByteArray& buffer, qint64 len);
    void getScreen(const QImage&);
    void getFrame(const cv::Mat&);
    void sendData(const QString&);

signals:
    void gettedAudio (const QString&);
    void gettedFrame (const QString&);
    void gettedScreen(const QString&);
};

#endif // CLIENT_H
