#ifndef STEWART_PLATFORM_H
#define STEWART_PLATFORM_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMap>

#include "ui_stewart_platform.h"
#include "ui_serial_settings_dialog.h"

#include "serial_settings_dialog.h"
#include "leap_event_listener.h"

namespace Ui {
class StewartPlatform;
}
class LeapEventListener;

class StewartPlatform : public QMainWindow
{
    Q_OBJECT

public:
    explicit StewartPlatform(QWidget *parent = 0);
    ~StewartPlatform();

    // Writes string to logging window.
    void Log(const QString& entry);

    // Constants
    const static int NUM_ACTUATORS = 6;
    const static int MIN_ACTUATOR_VALUE = 0;
    const static int MAX_ACTUATOR_VALUE = 1024;

    // Leap Motion listener
    LeapEventListener* leap;

public slots:
    // Converts actuator_positions vector into a single command string, sends over serial.
    // PRECONDITION: m_serial must be open
    void SendActuatorPositions(QVector<int> actuator_pos);
    void enableLeapMotion(bool c);
    void onLeapConnected(bool c);


private:

    // Variables representing platform state
    bool enable_leap;
    QVector<int> actuator_positions;

    // Functions to open/close m_serial
    void openSerialPort();
    void closeSerialPort();

    // Reads serial data and outputs to log.
    // PRECONDITION: m_serial must be open
    void readSerialData();

    // Writes given data to serial port
    // PRECONDITION: m_serial must be open
    void writeSerialData(const char* data);

    // UI-related vars
    Ui::StewartPlatform *ui;
    SerialSettingsDialog *m_settings = nullptr;
    QSerialPort* m_serial = nullptr;
    QVector<QSpinBox*> manual_fields;
    QVector<QSlider*> manual_sliders;
    QSpinBox* field;
    QSlider* slider;

private slots:
    void on_actionExit_triggered();
};

#endif // STEWART_PLATFORM_H
