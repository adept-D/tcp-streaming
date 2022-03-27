#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent),output(0), toOutput(QAudioDeviceInfo::defaultOutputDevice())
{
    server    = new Server;
    grid      = new QGridLayout;
    img_label = new QLabel;
    img_label->setMinimumSize(640,480);
    img_label->setStyleSheet("*{border: 1 solid black;}");

    connect(server, &Server::sendFrame, this, &Widget::setImageFromByte);
    connect(server, &Server::sendAudio, this, &Widget::setAudio);

    this->initAudioFormat();
    server->startServer();

    grid->addWidget(img_label,0,0,Qt::AlignCenter);
    this->setLayout(grid);
}


void Widget::setImageFromByte(QByteArray line, int rows, int cols, int format)
{
    try{
        cv::Mat frame(rows,cols,CV_8UC(format), (char*)line.data());
        if(frame.empty()){
            throw -1;
        }
        QImage image;

        if(format == 3){
            image= QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);

        }else if(format == 4){
            image= QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB32);
        }

        if(image.isNull()){
            throw -2;
        }
        image = image.scaled(640,480,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        img_label->setPixmap(QPixmap::fromImage(image));

        grid->addWidget(img_label,0,0,Qt::AlignCenter);
    }
    catch(int){
        std::cerr << "Empty img" << std::endl;
    }
}

void Widget::setAudio(QByteArray buffer, quint32 len)
{
    output->open(QIODevice::WriteOnly);
    output->write((char*)buffer.data(), len);
    output->close();
}


Widget::~Widget()
{
}

void Widget::initAudioFormat()
{
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::UnSignedInt );
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");


    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    if (!infoOut.isFormatSupported(format))
    {
        format = infoOut.nearestFormat(format);
    }

    audioOutput = new QAudioOutput(toOutput, format, this);
    output = audioOutput->start();

    audioOutput->setNotifyInterval(128);
}


