#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAudioOutput>
#include <QVBoxLayout>
#include <QTimer>
#include <QDataStream>
#include <QImage>
#include <QLabel>
#include "opencv2/opencv.hpp"
#include <QAudioDeviceInfo>
#include "image.h"

//Q_DECLARE_METATYPE(QByteArray&)
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer();
signals:
    void sendFrame(QByteArray, int, int, int);
    void sendAudio(QByteArray, quint32);

private slots:
    void slotReadyRead();

private:
    QTcpSocket* _socket;

    QByteArray imgData;      // Изображение с камеры
    QByteArray screenData;   // Изображение с экрана
    quint32  frameSize;
    quint32 rows, cols;


    QByteArray audioData;   // Аудио с микрофона
    quint32 audioLen;

    quint32 totalBytes;     // Сохраняем информацию об общем размере
    quint32 bytesReceived;  // Размер полученных данных

    QString type;           // Тип данных, которые пришли

private:
    virtual void incomingConnection(qintptr handle) override;

};

#endif // SERVER_H
