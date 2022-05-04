#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <QLabel>
#include <QFrame>
#include <QImage>
#include <QMainWindow>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QLabel *label = new QLabel(this);
    QFrame *frame = new QFrame(this);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage imdisplay;
    QTimer* timer;
    cv::VideoCapture cap;
    void DisplayImage();
    void showFrame(cv::Mat img);



//public slots:



private:
    void update();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
