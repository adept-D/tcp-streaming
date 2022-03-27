#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),camera(nullptr),audio(nullptr),screen(nullptr)
{
    init();

    label_frame->setMinimumSize(640,480);
    label_frame->setStyleSheet("*{border: 1 solid black;}");

    grid->addWidget(label_frame, 0, 0, 1 ,3, Qt::AlignCenter);
    grid->addWidget(btn_camera, 1,0);
    grid->addWidget(btn_audio, 1, 1);
    grid->addWidget(btn_screen, 1, 2);

    connect(btn_camera, &QPushButton::clicked, this, &Widget::onClickedBtnCamera);
    connect(btn_audio,  &QPushButton::clicked, this, &Widget::onClickedBtnAudio);
    connect(btn_screen, &QPushButton::clicked, this, &Widget::onClickedBtnDisplay);


    this->setLayout(grid);
}

Widget::~Widget()
{

}

void Widget::init()
{
    grid = new QGridLayout;

    btn_camera = new QPushButton("Streaming Camera");
    btn_audio  = new QPushButton("Streaming Audio");
    btn_screen = new QPushButton("Display");

    timer_camera  = new QTimer;
    timer_screen = new QTimer;
    timer_audio   = new QTimer;

    thread_camera = new QThread;
    thread_audio  = new QThread;
    thread_screen = new QThread;

    label_frame = new QLabel;

    camera = new Camera;
    screen = new Screen;
    client = new Client;
    audio  = new Audio;

    this->setThread(thread_camera, camera);
    this->setThread(thread_audio, audio);
    this->setThread(thread_screen, screen);


    client->connectServer();
    client->setAudio ( audio);
    client->setCamera(camera);
    client->setScreen(screen);

    //thread works
    connect(this, &Widget::readyCam,    camera, &Camera::updateFrame);
    connect(this, &Widget::readyAudio,  audio,  &Audio:: updateAudio);
    connect(this, &Widget::readyScreen, screen, &Screen::updateFrame);

    //deleteLater
    connect(grid, &QGridLayout::destroyed, grid, &QGridLayout::deleteLater);
    connect(label_frame, &QLabel::destroyed, label_frame, &QLabel::deleteLater);

    connect(btn_camera, &QPushButton::destroyed, btn_camera, &QPushButton::deleteLater);
    connect(btn_audio,  &QPushButton::destroyed, btn_audio,  &QPushButton::deleteLater);
    connect(btn_screen, &QPushButton::destroyed, btn_screen, &QPushButton::deleteLater);

    connect(timer_camera,  &QTimer::destroyed, timer_camera,  &QTimer::deleteLater);
    connect(timer_audio,   &QTimer::destroyed, timer_audio,   &QTimer::deleteLater);
    connect(timer_screen, &QTimer::destroyed, timer_screen, &QTimer::deleteLater);

    connect(thread_camera, &QThread::finished, thread_camera, &QThread::deleteLater);
    connect(thread_audio,  &QThread::finished, thread_audio,  &QThread::deleteLater);
    connect(thread_screen, &QThread::finished, thread_screen, &QThread::deleteLater);


    connect(camera, &Camera::destroyed, camera, &Camera::deleteLater);
    connect(screen, &Screen::destroyed, screen, &Screen::deleteLater);
    connect(client, &Client::destroyed, client, &Client::deleteLater);
    connect(audio,  &Audio::destroyed,  audio,  &Audio::deleteLater );


}

void Widget::onClickedBtnCamera()
{
    if(screen->isStart()){
        QMessageBox::warning(this, tr("Cam error"),tr("Завершите демонстрацию экрана"), QMessageBox::Cancel);
        return;
    }

    if( camera->isStart() ){
        btn_camera->setText("Streaming Camera");
        camera->stop();
        timer_camera->stop();
        return;
    }
    btn_camera->setText("Stop Camera");
    camera->start();

    connect(camera, &Camera::giveMatFromCamera, this, &Widget::slotGetQImage,Qt::QueuedConnection);
    connect(timer_camera, &QTimer::timeout, this, &Widget::readyCam, Qt::QueuedConnection);

    timer_camera->start(0);

    thread_camera->start();
}

void Widget::onClickedBtnAudio()
{
    if( audio->isStart() ){
        btn_audio->setText("Streaming Audio");
        audio->stop();
        timer_audio->stop();
        return;
    }

    btn_audio->setText("Stop Audio");
    audio->start();
    connect(audio, &Audio::destroyed, audio, &Audio::deleteLater);

    connect(timer_audio, &QTimer::timeout, this, &Widget::readyAudio, Qt::QueuedConnection);

    thread_audio->start();
    timer_audio->start(0);
}



void Widget::onClickedBtnDisplay()
{
    if(camera->isStart()){
        QMessageBox::warning(this, tr("Screen error"), tr("Завершите показ камеры"), QMessageBox::Cancel);
        return;
    }
    if( screen->isStart() ){
        btn_screen->setText("Streaming Display");
        screen->stop();
        timer_screen->stop();
        return;
    }
    screen->start();


    btn_screen->setText("Stop Display");

    connect(screen, &Screen::sendQImage, this, &Widget::slotGetScreen,Qt::QueuedConnection);
    connect(timer_screen, &QTimer::timeout, this, &Widget::readyScreen);
    timer_screen->start(50);

    thread_screen->start();
}

void Widget::slotGetQImage(const cv::Mat& mat)
{
    QImage image= QImage((uchar*) mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
    if(image.isNull()){
        qDebug() << "Image is empty" << Qt::endl;
    }
    label_frame->setPixmap(QPixmap::fromImage(image));
    grid->addWidget(label_frame, 0, 0, 1 ,3, Qt::AlignCenter);
}

void Widget::slotGetScreen(const QImage& image)
{
    label_frame->setPixmap(QPixmap::fromImage(image));
    grid->addWidget(label_frame, 0,0,1 , 3, Qt::AlignCenter);
}


void Widget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED( event )

    thread_camera->quit();
    thread_audio-> quit();
    thread_screen->quit();

    if(camera->isStart()){
        camera->stop();
    }
    else if(audio->isStart()){
        audio->stop();
    }
    else if(screen->isStart()){
        screen->stop();
    }

}


template<typename T>
void Widget::setThread(QThread* thread, const T& device)
{
    if(thread != nullptr){
        device->moveToThread(thread);
    }
}
