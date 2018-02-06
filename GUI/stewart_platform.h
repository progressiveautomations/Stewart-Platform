#ifndef STEWART_PLATFORM_H
#define STEWART_PLATFORM_H

#include <QMainWindow>

namespace Ui {
class StewartPlatform;
}

class StewartPlatform : public QMainWindow
{
    Q_OBJECT

public:
    explicit StewartPlatform(QWidget *parent = 0);
    ~StewartPlatform();

private:
    Ui::StewartPlatform *ui;

private slots:
    void on_actionExit_triggered();
};

#endif // STEWART_PLATFORM_H
