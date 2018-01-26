#include "stewart_platform.h"
#include "ui_stewart_platform.h"

StewartPlatform::StewartPlatform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StewartPlatform)
{
    ui->setupUi(this);
}

StewartPlatform::~StewartPlatform()
{
    delete ui;
}
