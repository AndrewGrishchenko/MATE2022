#include "RovCameraWidget.hpp"

#include <QCameraInfo>
#include <QCameraImageProcessing>
#include <QVBoxLayout>
#include <QDir>

using namespace std;

RovCameraWidget::RovCameraWidget(QWidget* parent)
    : QStackedWidget(parent)
    //, m_cameraView(new QCameraViewfinder(this))
    , m_cameraLabel(new QLabel(this))
    //, m_cameraCapture(new QtCameraCapture(this))
    //, m_cameraList(new QComboBox)
    //, m_refreshButton(new QPushButton(tr("Обновить")))
{
    //createCameraSelectionWidget();
    //createCameraViewWidget();
    //createConnections();
    //setCurrentIndex(WidgetType::CameraSelectionWidget);

    proccessTimer = new QTimer(this);
    proccessTimer->setInterval(50);
    proccessTimer->start();

    shipTimer = new QTimer(this);
    shipTimer->setInterval(50);
    shipTimer->start();

    connect(proccessTimer, &QTimer::timeout, this, &RovCameraWidget::proccessCamera);
    connect(shipTimer, &QTimer::timeout, this, &RovCameraWidget::countShip);

    addWidget(m_cameraLabel.data());
}

void RovCameraWidget::startCapture()
{
    cap.open(0);
    isProccessCamera = true;
}

void RovCameraWidget::proccessCamera()
{
    if (isProccessCamera)
    {
        cv::Mat originalImage;
        cap.read(originalImage);
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2RGB);
        QImage qOriginalImage((uchar*)originalImage.data, originalImage.cols,
            originalImage.rows, originalImage.step, QImage::Format_RGB888);
        m_cameraLabel.data()->setPixmap(QPixmap::fromImage(qOriginalImage));
        m_cameraLabel.data()->setScaledContents(true);
        m_cameraLabel.data()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

void RovCameraWidget::stopCapture()
{
    isProccessCamera = false;
    cap.release();
}

QCamera::Status RovCameraWidget::cameraStatus()
{
    if (m_camera) {
        return m_camera->status();
    }
    return QCamera::Status::UnavailableStatus;
}

void RovCameraWidget::createCameraSelectionWidget()
{
    QWidget* cameraWidget = new QWidget;

    QWidget* topSpacer = new QWidget;
    QWidget* bottomSpacer = new QWidget;
    QWidget* leftSpacer = new QWidget;
    QWidget* rightSpacer = new QWidget;

    topSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bottomSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    leftSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout* listLayout = new QHBoxLayout;

    listLayout->addWidget(leftSpacer);
    listLayout->addWidget(m_cameraLabel.data());
    listLayout->addWidget(m_cameraList.data());
    listLayout->addWidget(m_refreshButton.data());
    listLayout->addWidget(rightSpacer);

    m_refreshButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_cameraList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo& cameraInfo : availableCameras) {
        m_cameraList->addItem(cameraInfo.description());
    }

    QVBoxLayout* cameraListLayout = new QVBoxLayout;
    cameraListLayout->addWidget(topSpacer);
    cameraListLayout->addLayout(listLayout);
    cameraListLayout->addWidget(bottomSpacer);
    cameraWidget->setLayout(cameraListLayout);
    addWidget(cameraWidget);
}

void RovCameraWidget::createCameraViewWidget()
{
    addWidget(m_cameraView.data());
}

void RovCameraWidget::createConnections()
{
    QObject::connect(m_refreshButton.data(), &QPushButton::pressed, [this]() {
        m_cameraList->clear();
        const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
        for (const QCameraInfo& cameraInfo : availableCameras) {
            m_cameraList->addItem(cameraInfo.description());
        }
    });
}

//mosaic
void RovCameraWidget::takeMosaicPhoto()
{
    QDir dir("../photo/mosaic");
    if (!dir.exists()) dir.mkpath(".");

    if (isProccessCamera)
    {
        if (mosaicPhotoNum >= 9) mosaicPhotoNum = 1;
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
        image.save("../photo/mosaic/" + QString::number(mosaicPhotoNum) + ".png");
        if (mosaicPhotoNum >= 8) qDebug() << "all photos collected";
        mosaicPhotoNum += 1;
    }
}

void RovCameraWidget::mosaicPhotoBack()
{
    if (mosaicPhotoNum < 2) {
        mosaicPhotoNum -= 1;
        QDir dir("../photo/mosaic");
        dir.remove(QString::number(mosaicPhotoNum) + ".png");
    }
}


//shipLength
vector<vector<cv::Point>> findCnt(cv::Mat img, cv::Scalar low, cv::Scalar up) {
    cv::Mat hsv, mask;
    vector<vector<cv::Point>> cnt;
    cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);
    cv::inRange(hsv, low, up, mask);
    cv::findContours(mask, cnt, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    return cnt;
}

void RovCameraWidget::countShip()
{
    if (isCountShip && isProccessCamera)
    {
        cv::Mat img;
        cap.read(img);
    //    img = imread("work.jpg");
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        if(img.empty())return;

        cv::Mat mask, mask1, hsv;
        cv::Rect rect;
        cv::Scalar low = cv::Scalar(100, 50, 160);
        cv::Scalar up = cv::Scalar(140, 256, 256);
        cv::Scalar low1 = cv::Scalar(0, 50, 100);
        cv::Scalar up1 = cv::Scalar(60, 144, 197);
        vector<vector<cv::Point>> cnt;
        vector<vector<cv::Point>> cnt1;

        double m = 0, m1 = 0;
        int area;
        int w, h, r;

        cnt = findCnt(img, low, up);
        cnt1 = findCnt(img, low1, up1);

        for (auto count : cnt) {
            area = cv::contourArea(count);
            if (abs(area) < 1) continue;
            cv::approxPolyDP(count, count, cv::arcLength(count, true) * 0.02, true);
            rect = cv::boundingRect(count); w = rect.width; h = rect.height;
            if (w > h) m = w;
            else m = h;
        }

        for (auto count : cnt1) {
            area = cv::contourArea(count);
            if (abs(area) < 10) continue;
            cv::approxPolyDP(count, count, cv::arcLength(count, true) * 0.02, true);
            rect = cv::boundingRect(count); w = rect.width; h = rect.height;
            if (w > h) m1 = w;
            else m1 = h;
        }

        if (m != 0) r = m1 / m * 30;
        r += r * 0.25;
        if (40 < r and r < 180) qDebug() << r;
        else qDebug() << "not";
    }
}


//fishLength
void RovCameraWidget::takeFishPhoto()
{
    if (isProccessCamera)
    {
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
        image.save("../photo/fish/" + QString::number(currentFishNum) + ".png");
        currentFishNum += 1;
        if (currentFishNum == 7) { currentFishNum = 1; qDebug() << QString::number(fishCount) + " fish photos"; }
    }
}
