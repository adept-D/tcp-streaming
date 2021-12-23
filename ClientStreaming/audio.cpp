#include "audio.h"

const int Audio::BufferSize = 14096;

Audio::Audio(QObject *parent)
    : QObject{parent},
      toInput(QAudioDeviceInfo::defaultInputDevice()) ,
      input(0),
      buffer(BufferSize, 0)
{
    initializeAudio();
    createAudioInput();
    input = audioInput->start();

}

Audio::~Audio()
{
    input->close();
    audioInput->stop();
}

bool Audio::isStart()
{
    return this->isStarted;
}

void Audio::updateAudio()
{
    //Return if audio input is null
    if(!audioInput)
        return;

    //Check the number of samples in input buffer
    qint64 len = audioInput->bytesReady();

    //Limit sample size
    if(len > 4096)
        len = 4096;

     qint64 l  = input->read(buffer.data(), len);

    if(l > 0){
        emit giveData(buffer, len);
    }

}

void Audio::start()
{
    buffer.resize(BufferSize);
    if(audioInput->state() == QAudio::SuspendedState){
        audioInput->resume();
    }
    this->isStarted = true;
    emit started();
}

void Audio::stop()
{
    this->isStarted = false; // Сообщает о том, что аудио выключено/остановлено
    this->buffer.clear();
    audioInput->suspend(); // остановка аудио
    emit finished();
}

void Audio::initializeAudio()
{

    format.setSampleRate(8000); //set frequency to 8000
    format.setChannelCount(1); //set channels to mono
    format.setSampleSize(16); //set sample sze to 16 bit
    format.setSampleType(QAudioFormat::SignedInt ); //Sample type as usigned integer sample //UnSignedInt
    format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(format))
    {
        //Default format not supported - trying to use nearest
        format = infoIn.nearestFormat(format);
    }
}


void Audio::createAudioInput()
{
    if (input != 0) {
        disconnect(input, 0, this, 0);
        input = 0;
    }
    audioInput = new QAudioInput(toInput, format, this);
    connect(audioInput, &QAudioInput::destroyed, audioInput, &QAudioInput::deleteLater);
}


