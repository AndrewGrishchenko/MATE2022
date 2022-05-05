#include "MainWindow.hpp"
#include "RovSingleton.hpp"

#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>

QTextEdit* MainWindow::logWidget = nullptr;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_telimetryWidget(new RovTelimetryWidget(this))
    , m_cameraWidget(new RovCameraWidget(this))
    , m_scaleWidget(new ScaleFactorWidget(this))
    , m_joystick(new SfJoystick(this))
    , m_regulatorsWidget(new RegulatorsWidget(this))
    , m_debugDialog(new DebugDialog(this))
    , m_logFileWidget(new LogWidget(this))
{
    logWidget = new QTextEdit;
    createActions();
    createMenus();
    createConnections();
    createDocks();
    setCentralWidget(m_cameraWidget.data());
    setMinimumSize(640, 480);

    transectTimer = new QTimer(this);
    transectTimer->setInterval(20);
    transectTimer->start();

    connect(transectTimer, &QTimer::timeout, this, &MainWindow::transect);
}

MainWindow::~MainWindow() {}

void MainWindow::createMenus()
{
    QMenu* camera = menuBar()->addMenu(tr("Камера"));
    camera->addAction(m_startCameraAct.data());
    camera->addAction(m_stopCameraAct.data());
    camera->addAction(m_switchCameraAct.data());

    QMenu* settings = menuBar()->addMenu(tr("Настройки"));
    settings->addAction(m_openJoystickSettings.data());
    settings->addAction(m_openDebugDialog.data());

    QMenu* mosaic = menuBar()->addMenu(tr("Мозаика"));
    mosaic->addAction(m_takeMosaicPhoto.data());
    mosaic->addAction(m_mosaicPhotoBack.data());
    mosaic->addAction(m_sendMosaicToFlash.data());

    QMenu* ship = menuBar()->addMenu(tr("Корабль"));
    ship->addAction(m_startStopCountShip.data());

    QMenu* fish = menuBar()->addMenu(tr("Рыбов"));
    fish->addAction(m_takeFishPhoto.data());
    fish->addAction(m_fishPhotoBack.data());
    fish->addAction(m_sendFishToFlash.data());

    QMenu* env = menuBar()->addMenu(tr("Env"));
    env->addAction(m_clearAllPhotos.data());
    env->addAction(m_clearMosaicPhotos.data());
    env->addAction(m_clearFishPhotos.data());
    env->addAction(m_clearAllFlash.data());
    env->addAction(m_clearMosaicFlash.data());
    env->addAction(m_clearFishFlash.data());

    QMenu* autos = menuBar()->addMenu(tr("Autos"));
    autos->addAction(m_startStopTransect.data());
}

void MainWindow::createDocks()
{
    QMenu* view = menuBar()->addMenu(tr("&Вид"));

    QDockWidget* consDock = new QDockWidget(tr("Телеметрия"), this);
    consDock->setObjectName("TelimetryDockWidget");
    consDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    consDock->setWidget(m_telimetryWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, consDock);
    view->addAction(consDock->toggleViewAction());

    QDockWidget* logDock = new QDockWidget(tr("Информация"), this);
    logDock->setObjectName("LogDockWidget");
    logDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    logDock->setWidget(logWidget);
    logWidget->setFont(QFont("Consolas"));
    addDockWidget(Qt::LeftDockWidgetArea, logDock);
    view->addAction(logDock->toggleViewAction());

    QDockWidget* scaleDock = new QDockWidget(tr("Ограничитель тяги"), this);
    scaleDock->setObjectName("ScaleWidget");
    scaleDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    scaleDock->setWidget(m_scaleWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, scaleDock);
    view->addAction(scaleDock->toggleViewAction());

    QDockWidget* regulatorsDock = new QDockWidget(tr("Регуляторы"), this);
    regulatorsDock->setObjectName("RegulatorsWidget");
    regulatorsDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    regulatorsDock->setWidget(m_regulatorsWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, regulatorsDock);
    view->addAction(regulatorsDock->toggleViewAction());

    QDockWidget* logFileDock = new QDockWidget(tr("Логи"), this);
    logFileDock->setObjectName("RegulatorsWidget");
    logFileDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);
    logFileDock->setWidget(m_logFileWidget.data());
    addDockWidget(Qt::LeftDockWidgetArea, logFileDock);
    view->addAction(logFileDock->toggleViewAction());
}

void MainWindow::createConnections()
{
    QObject::connect(m_startCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->startCapture();
    });
    QObject::connect(m_stopCameraAct.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->stopCapture();
    });
    QObject::connect(m_switchCameraAct.data(), &QAction::triggered, [this](bool) {
        RovSingleton::instance()->control().cameraIndex = (RovSingleton::instance()->control().cameraIndex + 1) % 2;
    });
    QObject::connect(m_openJoystickSettings.data(), &QAction::triggered, [this](bool) {
        m_joystick.data()->settingsDialog()->show();
    });
    QObject::connect(m_openDebugDialog.data(), &QAction::triggered, [this](bool) {
        m_debugDialog.data()->show();
    });



    //mosaic
    QObject::connect(m_takeMosaicPhoto.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->takeMosaicPhoto();
    });
    QObject::connect(m_mosaicPhotoBack.data(), &QAction::triggered, [this](bool) {
        mosaicPhotoBack();
    });
    QObject::connect(m_sendMosaicToFlash.data(), &QAction::triggered, [this](bool) {
        sendMosaicToFlash();
    });


    //shipLength
    QObject::connect(m_startStopCountShip.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->countShip();
    });


    //fishLength
    QObject::connect(m_takeFishPhoto.data(), &QAction::triggered, [this](bool) {
        m_cameraWidget->takeFishPhoto();
    });
    QObject::connect(m_fishPhotoBack.data(), &QAction::triggered, [this](bool) {
        fishPhotoBack();
    });
    QObject::connect(m_sendFishToFlash.data(), &QAction::triggered, [this](bool) {
        sendFishToFlash();
    });


    //env
    QObject::connect(m_clearAllPhotos.data(), &QAction::triggered, [this](bool) {
        clearAllPhotos();
    });
    QObject::connect(m_clearMosaicPhotos.data(), &QAction::triggered, [this](bool) {
        clearMosaicPhotos();
    });
    QObject::connect(m_clearFishPhotos.data(), &QAction::triggered, [this](bool) {
        clearFishPhotos();
    });
    QObject::connect(m_clearAllFlash.data(), &QAction::triggered, [this](bool) {
        clearAllFlash();
    });
    QObject::connect(m_clearMosaicFlash.data(), &QAction::triggered, [this](bool) {
        clearMosaicFlash();
    });
    QObject::connect(m_clearFishFlash.data(), &QAction::triggered, [this](bool) {
        clearFishFlash();
    });


    //transect
    QObject::connect(m_startStopTransect.data(), &QAction::triggered, [this](bool) {
        keepingDepth = RovSingleton::instance()->telimetry().depth;
        keepingYaw = RovSingleton::instance()->telimetry().yaw;
        isTransect = !isTransect;
    });
}

void MainWindow::createActions()
{
    m_startCameraAct.reset(new QAction(tr("Начать захват изображения"), this));
    m_stopCameraAct.reset(new QAction(tr("Остановить захват изображения"), this));
    m_switchCameraAct.reset(new QAction(tr("Переключить камеру (мультиплексор)"), this));
    m_openJoystickSettings.reset(new QAction(tr("Настройки джойстика"), this));
    m_openDebugDialog.reset(new QAction(tr("Отладка движителей"), this));

    //mosaic
    m_takeMosaicPhoto.reset(new QAction(tr("Фото"), this));
    m_mosaicPhotoBack.reset(new QAction(tr("Назад"), this));
    m_sendMosaicToFlash.reset(new QAction(tr("Отправить"), this));

    //shipLength
    m_startStopCountShip.reset(new QAction(tr("Измерение корабля"), this));

    //fishLength
    m_takeFishPhoto.reset(new QAction(tr("Фото"), this));
    m_fishPhotoBack.reset(new QAction(tr("Назад"), this));
    m_sendFishToFlash.reset(new QAction(tr("Отправить"), this));

    //env
    m_clearAllPhotos.reset(new QAction(tr("Очистить все"), this));
    m_clearMosaicPhotos.reset(new QAction(tr("Очистить мозаику"), this));
    m_clearFishPhotos.reset(new QAction(tr("Очистить рыбов"), this));
    m_clearAllFlash.reset(new QAction(tr("Очистить все флешка"), this));
    m_clearMosaicFlash.reset(new QAction(tr("Очистить мозаику флешка"), this));
    m_clearFishFlash.reset(new QAction(tr("Очистить рыбов флешка"), this));

    //transect
    m_startStopTransect.reset(new QAction(tr("Трансекта (помогите)"), this));
}


//mosaic
void MainWindow::mosaicPhotoBack()
{
    if (m_cameraWidget->mosaicPhotoNum < 2) {
        m_cameraWidget->mosaicPhotoNum -= 1;
        QDir dir("../photo/mosaic");
        dir.remove(QString::number(m_cameraWidget->mosaicPhotoNum) + ".png");
    }
}

void MainWindow::sendMosaicToFlash()
{
    QDir dir(flashPath + "photo/mosaic");
    if (dir.exists())
    {
        for (int i = 0; i < 9; i++) {
            QFile::copy("../photo/mosaic/" + QString::number(i) + ".png", flashPath + "photo/mosaic/" + QString::number(i) + ".png");
        }
    }
}


//fishLength
void MainWindow::fishPhotoBack()
{
    if (m_cameraWidget->currentFishNum > 1)
    {
        QDir dir("../photo/fish/");
        if (dir.exists()) {
            dir.remove(QString::number(m_cameraWidget->currentFishNum) + ".png");
            m_cameraWidget->currentFishNum -= 1;
        }
    }
}

void MainWindow::sendFishToFlash()
{
    QDir dir(flashPath + "photo/fish");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++) {
            QFile::copy("../photo/fish/" + QString::number(i) + ".png", flashPath + "photo/fish/" + QString::number(i) + ".png");
        }
    }
}


//env
void MainWindow::clearAllPhotos()
{
    clearMosaicPhotos();
    clearFishPhotos();
}

void MainWindow::clearMosaicPhotos()
{
    QDir dir("../photo/mosaic");
    if (dir.exists())
    {
        for (int i = 1; i < 9; i++) {
            dir.remove(QString::number(i) + ".png");
        }
        m_cameraWidget->mosaicPhotoNum = 1;
    }
}

void MainWindow::clearFishPhotos()
{
    QDir dir("../photo/fish");
    if (dir.exists())
    {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++) {
            dir.remove(QString::number(i) + ".png");
        }
        m_cameraWidget->currentFishNum = 1;
    }
}

void MainWindow::clearAllFlash()
{
    clearMosaicFlash();
    clearFishFlash();
}

void MainWindow::clearMosaicFlash()
{
    QDir dir(flashPath + "photo/mosaic");
    if (dir.exists()) {
        for (int i = 1; i < 9; i++) {
            dir.remove(QString::number(i) + ".png");
        }
    }
}

void MainWindow::clearFishFlash()
{
    QDir dir(flashPath + "photo/fish");
    if (dir.exists()) {
        for (int i = 1; i <= m_cameraWidget->fishCount; i++)
        {
            dir.remove(QString::number(i) + ".png");
        }
    }
}


//hotkeys
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Z:
            qDebug() << "Z";
            break;
        case Qt::Key_X:
            qDebug() << "X";
            break;
        case Qt::Key_C:
            qDebug() << "C";
            break;
    }
}


//transect
void MainWindow::transect()
{
    if (isTransect)
    {
//        float current_time = QDateTime::currentMSecsSinceEpoch();

//        float depth_error = RovSingleton::instance()->telimetry().depth - keepingDepth;
//        float yaw_error = RovSingleton::instance()->telimetry().yaw - keepingYaw;

//        float z_power = 0;
//        float power1 = 0;
//        float power2 = 0;
//        float power3 = 0;
//        float power4 = 0;

//        float power_value = error * 70;
//        float diff_value = 5 / (current_time - m_depth_prev_time) * (error - m_depth_prev_error);

//        m_power = clamp(power_value + diff_value, 100, -100) * 0.6;

//        RovSingleton::instance()->control().axisZ = static_cast<qint8>(m_power);

//        m_depth_prev_time = current_time;
//        m_depth_prev_error = error;
    }
}
