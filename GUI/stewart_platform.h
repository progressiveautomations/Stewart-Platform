#ifndef STEWART_PLATFORM_H
#define STEWART_PLATFORM_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>

#include "ui_stewart_platform.h"
#include "ui_settingsdialog.h"

#include "serial_settings_dialog.h"
#include "leap_event_listener.h"

namespace Ui {
class StewartPlatform;
}

class StewartPlatform : public QMainWindow
{
    Q_OBJECT

public:
    explicit StewartPlatform(QWidget *parent = 0);
    ~StewartPlatform();

    // Writes string to logging window.
    void Log(const QString& entry);

    // Writes given data to serial port
    // PRECONDITION: m_serial must be open
    void writeSerialData(const char* data);

private:
    // Functions to open/close m_serial
    void openSerialPort();
    void closeSerialPort();

    // Reads serial data and outputs to log.
    // PRECONDITION: m_serial must be open
    void readSerialData();

    Ui::StewartPlatform *ui;
    SerialSettingsDialog *m_settings = nullptr;
    QSerialPort* m_serial = nullptr;

    bool enable_leap;
    QList<int> actuator_positions;
    const int NUM_ACTUATORS = 6;
    const int MAX_OUTPUT_DATA_SIZE = 30;

private slots:
    void on_actionExit_triggered();
};

#endif // STEWART_PLATFORM_H
