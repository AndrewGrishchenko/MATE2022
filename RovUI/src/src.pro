QT       += core gui network multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RovUI
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Crc.cpp \
    RovTelimetryWidget.cpp \
    RovCameraWidget.cpp \
    ScaleFactorWidget.cpp \
    SfJoystick.cpp \
    SfJoystickSettingsDialog.cpp \
    RovSingleton.cpp \
    RovUdpConnection.cpp \
    RegulatorsWidget.cpp \
    DebugDialog.cpp \
    LogWidget.cpp

HEADERS += \
    MainWindow.hpp \
    CallOnce.hpp \
    Singleton.hpp \
    Crc.hpp \
    RovTelimetryWidget.hpp \
    RovCameraWidget.hpp \
    ScaleFactorWidget.hpp \
    SfJoystick.hpp \
    SfJoystickSettingsDialog.hpp \
    RovSingleton.hpp \
    RovUdpConnection.hpp \
    RovDataTypes.hpp \
    RegulatorsWidget.hpp \
    DebugDialog.hpp \
    LogWidget.hpp

include(deps/deps.pri)

RESOURCES +=

#INCLUDEPATH += $(HOME)/opencv-build/build2/include/opencv4/

#LIBS += -L$(HOME)/opencv-build/build2/lib \
#        -lopencv_core \
#        -lopencv_imgproc \
#        -lopencv_imgcodecs \
#        -lopencv_highgui \
#        -lopencv_objdetect \
#        -lopencv_videoio

#LIBS += $(HOME)/opencv-build/build/lib/libopencv_core.so
#LIBS += $(HOME)/opencv-build/build/lib/libopencv_imgproc.so
#LIBS += $(HOME)/opencv-build/build/lib/libopencv_imgcodecs.so
#LIBS += $(HOME)/opencv-build/build/lib/libopencv_highgui.so
#LIBS += $(HOME)/opencv-build/build/lib/libopencv_objdetect.so
