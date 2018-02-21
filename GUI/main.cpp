#include "stewart_platform.h"
#include "leap_event_listener.h"
#include "ui_stewart_platform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StewartPlatform w;
    Controller controller;
    controller.addListener(*(w.leap));
    w.show();
    int ac = a.exec();
    controller.removeListener(*(w.leap));
    return ac;
}
