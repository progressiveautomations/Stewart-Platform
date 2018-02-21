#ifndef STEWART_PLATFORM_H
#define STEWART_PLATFORM_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QMap>

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

    // Converts actuator_positions vector into a single command string, sends over serial.
    // PRECONDITION: m_serial must be open
    void SendActuatorPositions();

private:
    // Constants
    const int NUM_ACTUATORS = 6;
    const int MAX_OUTPUT_DATA_SIZE = 30;
    const int MAX_ACTUATOR_VALUE = 1024;
    const int MIN_ACTUATOR_VALUE = 0;

    // Variables representing platform state
    bool enable_leap;
    QVector<int> actuator_positions;

    // Functions to open/close m_serial
    void openSerialPort();
    void closeSerialPort();

    // Reads serial data and outputs to log.
    // PRECONDITION: m_serial must be open
    void readSerialData();

    // UI-related vars
    Ui::StewartPlatform *ui;
    SerialSettingsDialog *m_settings = nullptr;
    QSerialPort* m_serial = nullptr;
    QMap<QSpinBox*, QSlider*> manual_adjust;

private slots:
    void on_actionExit_triggered();
};

#endif // STEWART_PLATFORM_H
