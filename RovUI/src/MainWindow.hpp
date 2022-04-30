#pragma once

#include "DebugDialog.hpp"
#include "LogWidget.hpp"
#include "RegulatorsWidget.hpp"
#include "RovCameraWidget.hpp"
#include "RovTelimetryWidget.hpp"
#include "ScaleFactorWidget.hpp"
#include "SfJoystick.hpp"
#include <QAction>
#include <QMainWindow>
#include <QTextEdit>
#include <QDir>
#include <QString>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);

    ~MainWindow();
    static QTextEdit* logWidget;

private:
    void createMenus();

    void createDocks();

    void createConnections();

    void createActions();

    void takePhoto();

    void sendPhotoToFlash();

    QScopedPointer<QAction> m_startCameraAct;
    QScopedPointer<QAction> m_stopCameraAct;
    QScopedPointer<QAction> m_switchCameraAct;
    QScopedPointer<QAction> m_openJoystickSettings;
    QScopedPointer<QAction> m_openDebugDialog;

    QScopedPointer<RovTelimetryWidget> m_telimetryWidget;
    QScopedPointer<RovCameraWidget> m_cameraWidget;
    QScopedPointer<ScaleFactorWidget> m_scaleWidget;
    QScopedPointer<SfJoystick> m_joystick;
    QScopedPointer<RegulatorsWidget> m_regulatorsWidget;
    QScopedPointer<DebugDialog> m_debugDialog;
    QScopedPointer<LogWidget> m_logFileWidget;

    //mosaic
    QScopedPointer<QAction> m_takePhoto;
    QScopedPointer<QAction> m_onePhotoBack;
    QScopedPointer<QAction> m_clearPhotos;
    QScopedPointer<QAction> m_sendPhotoToFlash;

    //andrew ubuntu config
    QString flashPath = "/media/andrew/FLASH/";
};
