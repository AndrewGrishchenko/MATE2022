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
    //const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    //if (availableCameras.empty()) {
    //    qWarning() << "Нет доступных камер!";
    //    return;
    //}

//    cv::VideoCapture camera;
//    int device_count = 0;
//    while (true) {
//        if (!camera.open(device_count++)) {
//            break;
//        }
//    }
//    ca1era.release();
//    device_count -= 1;
//    qDebug() << m_cameraList->currentIndex();

    //qDebug() << 1;
    cap.open(0);
    isProccessCamera = true;

    //while (true) {
    //    cv::Mat originalImage;
    //    cap.read(originalImage);
    //    cv::imshow("image", originalImage);
    //    cv::waitKey(50);
    //}
//    camera.open(cv::CAP_V4L2);
    //qDebug() << 2;
    //cv::Mat originalImage;
    //cap.read(originalImage);
    //qDebug() << 3;
    //cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2RGB);
    //QImage qOriginalImage((uchar*)originalImage.data, originalImage.cols,
    //    originalImage.rows, originalImage.step, QImage::Format_RGB888);
    //m_cameraLabel.data()->setPixmap(QPixmap::fromImage(qOriginalImage));
    //m_cameraLabel.data()->setScaledContents(true);
    //m_cameraLabel.data()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    //QCameraInfo cameraInfo = availableCameras.at(m_cameraList->currentIndex());

    //m_camera.reset(new QCamera(cameraInfo));
    //m_camera->setViewfinder(m_cameraView.data());
    ////m_camera->setViewfinder(m_cameraCapture.data());
    //m_camera->start();
    //setCurrentIndex(WidgetType::CameraViewWidget);
}

void RovCameraWidget::proccessCamera()
{
    if (isProccessCamera)
    {
        cv::Mat originalImage;
        cap.read(originalImage);
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2RGB);
        //cv::imshow("image", originalImage);
        QImage qOriginalImage((uchar*)originalImage.data, originalImage.cols,
            originalImage.rows, originalImage.step, QImage::Format_RGB888);
        m_cameraLabel.data()->setPixmap(QPixmap::fromImage(qOriginalImage));
        m_cameraLabel.data()->setScaledContents(true);
        m_cameraLabel.data()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    }
}

void RovCameraWidget::stopCapture()
{
//    if (!m_camera) {
//        return;
//    }

//    m_camera->stop();
//    setCurrentIndex(WidgetType::CameraSelectionWidget);
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
void RovCameraWidget::takePhoto()
{
    if (isProccessCamera)
    {
        if (mosaicPhotoNum >= 9) mosaicPhotoNum = 1;
        cv::Mat img;
        cap.read(img);
        cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
        QImage image(img.data, img.cols, img.rows, QImage::Format_RGB888);
        image.save("../photo/" + QString::number(mosaicPhotoNum) + ".png");
        if (mosaicPhotoNum >= 8) qDebug() << "all photos collected";
        mosaicPhotoNum += 1;
    }
}

void RovCameraWidget::onePhotoBack()
{
    if (!mosaicPhotoNum < 2) {
        mosaicPhotoNum -= 1;
        QDir dir("../photo/");
        dir.remove(QString::number(mosaicPhotoNum) + ".png");
    }
}

void RovCameraWidget::clearPhotos()
{
    mosaicPhotoNum = 1;
    QDir dir("../photo/");
    if (dir.exists()) {
        for (int i = 1; i < 9; i++) {
            dir.remove(QString::number(i) + ".png");
        }
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
    //    qDebug() << cnt.size() << cnt1.size();
    //    cvtColor(img, hsv, COLOR_RGB2HSV);
    //    inRange(hsv, low, up, mask);
    //    findContours(mask, cnt, RETR_TREE, CHAIN_APPROX_SIMPLE);
    //    inRange(hsv, low1, up1, mask1);
    //    findContours(mask1, cnt1, RETR_TREE, CHAIN_APPROX_SIMPLE);

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

    //    cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);
    //    cv::inRange(hsv, low1, up1, hsv);
    //    cv::imshow("img", hsv);
    //    showFrame(img);
//        cv::waitKey(1);
    }
}
