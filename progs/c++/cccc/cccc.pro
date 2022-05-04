QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

CONFIG-= windows
QMAKE_LFLAGS += $$QMAKE_LFLAGS_WINDOWS
#INCLUDEPATH += "C:\Qt\Qt5.9.9\Tools\SDL-1.2.15_32\SDL-1.2.15\include"
#LIBS += -L "C:\Qt\Qt5.9.9\Tools\SDL-1.2.15_32\SDL-1.2.15\lib" -lSDL

#INCLUDEPATH += "C:\Qt\Qt5.9.9\Tools\opencv\build\include"
#LIBS += -L"C:\Qt\Qt5.9.9\Tools\opencv-build\lib" \
#        -llibopencv_calib3d440.dll \
#        -llibopencv_core440.dll \
#        -llibopencv_dnn440.dll \
#        -llibopencv_features2d440.dll \
#        -llibopencv_flann440.dll \
#        -llibopencv_highgui440.dll \
#        -llibopencv_imgcodecs440.dll \
#        -llibopencv_imgproc440.dll \
#        -llibopencv_ml440.dll \
#        -llibopencv_objdetect440.dll \
#        -llibopencv_photo440.dll \
#        -llibopencv_stitching440.dll \
#        -llibopencv_video440.dll \
#        -llibopencv_videoio440.dll \


INCLUDEPATH += $(HOME)/opencv-build/build2/include/opencv4/

LIBS += -L$(HOME)/opencv-build/build2/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_objdetect \
        -lopencv_videoio

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
