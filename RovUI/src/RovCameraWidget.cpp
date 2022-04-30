#include "RovCameraWidget.hpp"

#include <QCameraInfo>
#include <QCameraImageProcessing>
#include <QVBoxLayout>
#include <QDir>

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

    connect(proccessTimer, &QTimer::timeout, this, &RovCameraWidget::proccessCamera);

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
//    camera.release();
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
