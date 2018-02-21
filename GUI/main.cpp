#include "stewart_platform.h"
#include "ui_stewart_platform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StewartPlatform w;
    w.show();

    return a.exec();
}
