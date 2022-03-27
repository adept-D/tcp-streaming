#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QTcpSocket>
#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include "opencv2/opencv.hpp"
#include <QPixmap>
#include <QApplication>
#include <QMessageBox>

#include <QThread>
#include <QEvent>
#include <QMetaObject>

#include "camera.h"
#include "audio.h"
#include "screen.h"
#include "client.h"


Q_DECLARE_METATYPE(cv::Mat)
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QGridLayout* grid;
    QLabel* label_frame;

    QPushButton* btn_camera;
    QPushButton* btn_audio;
    QPushButton* btn_screen;

    Camera* camera;
    Audio*  audio;
    Screen* screen;
    Client* client;

    QTimer* timer_camera;
    QTimer* timer_screen;
    QTimer* timer_audio;

    QThread* thread_camera;
    QThread* thread_audio;
    QThread* thread_screen;

private:
    void init();
    template<typename T>
    void setThread(QThread*,const T&);

private slots:
    void onClickedBtnCamera();
    void onClickedBtnAudio();
    void onClickedBtnDisplay();

    void slotGetQImage(const cv::Mat&);
    void slotGetScreen(const QImage&);

signals:
    void readyCam();
    void readyAudio();
    void readyScreen();
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};
#endif // WIDGET_H
