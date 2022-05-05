#pragma once

#include <QCamera>
#include <QCameraViewfinder>
//#include "qtcameracapture.h"
#include <QComboBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <vector>

class RovCameraWidget : public QStackedWidget {
    Q_OBJECT
public:
    explicit RovCameraWidget(QWidget* parent = nullptr);

    bool isCountShip = false;
    int fishCount = 6;
    int currentFishNum = 1;
    int mosaicPhotoNum = 1;
signals:
public slots:
    void startCapture();
    void stopCapture();
    QCamera::Status cameraStatus();

    //mosaic
    void takeMosaicPhoto();
    void mosaicPhotoBack();

    //shipLength
    void countShip();

    //fishLength
    void takeFishPhoto();
private:
    enum WidgetType {
        CameraSelectionWidget = 0,
        CameraViewWidget = 1
    };

    void createCameraSelectionWidget();

    void createCameraViewWidget();

    void createConnections();

    QScopedPointer<QCameraViewfinder> m_cameraView;
    //QScopedPointer<QtCameraCapture> m_cameraCapture;
    QScopedPointer<QCamera> m_camera;
    QScopedPointer<QComboBox> m_cameraList;
    QScopedPointer<QPushButton> m_refreshButton;
    QScopedPointer<QLabel> m_cameraLabel;

    QTimer* proccessTimer;
    void proccessCamera();
    bool isProccessCamera = false;
    cv::VideoCapture cap;

    //shipLength
    QTimer* shipTimer;
};
