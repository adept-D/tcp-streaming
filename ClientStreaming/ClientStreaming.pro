QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH  += C:\libs\opencv\build\include

LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_core454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_video454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_videoio454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_highgui454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_imgproc454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_imgcodecs454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_features2d454.dll
LIBS  +=  C:\libs\opencv\OpneCV_bin\bin\libopencv_calib3d454.dll




SOURCES += \
    audio.cpp \
    camera.cpp \
    client.cpp \
    main.cpp \
    screen.cpp \
    widget.cpp

HEADERS += \
    audio.h \
    camera.h \
    client.h \
    screen.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
