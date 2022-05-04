#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QTimer"
using namespace cv;
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cap.open(0);
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->start(25);
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, &MainWindow::DisplayImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showFrame(Mat img) {
    QImage imdisplay((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(imdisplay));
    ui->label->setScaledContents( true );
    ui->label->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
}

vector<vector<Point>> findCnt(Mat img, Scalar low, Scalar up) {
    Mat hsv, mask;
    vector<vector<Point> > cnt;
    cvtColor(img, hsv, COLOR_RGB2HSV);
    inRange(hsv, low, up, mask);
    findContours(mask, cnt, RETR_TREE, CHAIN_APPROX_SIMPLE);
    return cnt;
}

void MainWindow::DisplayImage()
{

    Mat img; /* cap.read(img); */ img = imread("work.jpg"); cvtColor(img, img, COLOR_BGR2RGB);
    if(img.empty())return;

    Mat mask, mask1, hsv;
    Rect rect;
    Scalar low = Scalar(100, 50, 160);
    Scalar up = Scalar(140, 256, 256);
    Scalar low1 = Scalar(0, 50, 100);
    Scalar up1 = Scalar(60, 144, 197);
    vector<vector<Point> > cnt;
    vector<vector<Point> > cnt1;

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
        area = contourArea(count);
        if (abs(area) < 1) continue;
        approxPolyDP(count, count, arcLength(count, true) * 0.02, true);
        rect = boundingRect(count); w = rect.width; h = rect.height;
        if (w > h) m = w;
        else m = h;
    }

    for (auto count : cnt1) {
        area = contourArea(count);
        if (abs(area) < 10) continue;
        approxPolyDP(count, count, arcLength(count, true) * 0.02, true);
        rect = boundingRect(count); w = rect.width; h = rect.height;
        if (w > h) m1 = w;
        else m1 = h;
    }

    if (m != 0) r = m1 / m * 30;
    r += r * 0.25;
    if (40 < r and r < 180) cout << r << endl;
    else cout << "not" << endl;

//    cv::cvtColor(img, hsv, cv::COLOR_RGB2HSV);
//    cv::inRange(hsv, low1, up1, hsv);
//    cv::imshow("img", hsv);
    showFrame(img);
    waitKey(50);

}


