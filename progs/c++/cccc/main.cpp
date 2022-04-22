#include "mainwindow.h"
#include <QApplication>
#undef main
using namespace  cv;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
