#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QByteArray>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QDebug>
#include <QThread>
#include <QTimer>

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(QObject *parent = nullptr);
     ~Audio();
    bool isStart();
    void updateAudio();
    void stop();
private: //variables
    bool isStarted = false;
    QTimer* audioTimer;
    QAudioInput* audioInput;

    QAudioFormat format;

    QAudioDeviceInfo toInput;

    QIODevice* input;
    QByteArray buffer;

    static const int BufferSize;

private: //func
    void createAudioInput();
    void initializeAudio();

public slots:
    void start();

signals:
    void giveData(const QByteArray&, quint64);
    void finished();
    void started();
};

#endif // AUDIO_H
