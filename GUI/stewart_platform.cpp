#include "stewart_platform.h"

StewartPlatform::StewartPlatform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StewartPlatform),
    m_settings(new SerialSettingsDialog),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);
    // Setup map of manual control elements
    actuator_positions = {0, 0, 0, 0, 0, 0};
    manual_adjust = {{ui->field_1, ui->slider_1}, {ui->field_2, ui->slider_2}, {ui->field_3, ui->slider_3}, {ui->field_4, ui->slider_4}, {ui->field_5, ui->slider_5}, {ui->field_6, ui->slider_6}};

    // Index
    int i = 0;
    QMapIterator<QSpinBox*, QSlider*> iter(manual_adjust);
    // Setup signals for each pair
    while(iter.hasNext())
    {
        iter.next();
        // Set max and min for each actuator
        iter.key()->setMaximum(MAX_ACTUATOR_VALUE);
        iter.value()->setMaximum(MAX_ACTUATOR_VALUE);
        iter.key()->setMinimum(MIN_ACTUATOR_VALUE);
        iter.value()->setMinimum(MIN_ACTUATOR_VALUE);

        /* Signals */
        // Slider updates spinbox and spinbox updates sliders
        connect(iter.key(), QOverload<int>::of(&QSpinBox::valueChanged), iter.value(), &QSlider::setValue);
        connect(iter.value(), &QSlider::valueChanged, iter.key(), &QSpinBox::setValue);

        // Slider updates actuator_positions (lambda selects appropriate index)
        connect(iter.value(), &QSlider::valueChanged, this, [=](int j){this->actuator_positions[i] = j;});
        ++i;
    }

    // Initialize status labels
    ui->label_serial_val->setText(tr("Disconnected"));
    ui->label_leap_val->setText(tr("Disconnected"));

    // Send button deinitialized
    ui->button_send->setEnabled(false);

    // Leap Motion checkbox disables actuator box, update leap_enabled variable
    connect(ui->enable_leap_motion, &QCheckBox::toggled, ui->actuatorBox, [=](){ ui->actuatorBox->setEnabled(!ui->enable_leap_motion->isChecked()); this->enable_leap = ui->enable_leap_motion->isChecked();});

    /** Connect the serial-releated signals **/
    // Serial logging
    connect(m_serial, &QSerialPort::readyRead, this, &StewartPlatform::readSerialData);

    // Serial Settings dialog
    connect(ui->actionSelect_COM_port, &QAction::triggered, m_settings, &SerialSettingsDialog::show);

    // Connect to serial port
    connect(m_settings, &SerialSettingsDialog::SettingsUpdated, this, &StewartPlatform::openSerialPort);

    // Disconnect from serial port if something happens
    // todo: make dedicated error handling function, for now disconnecting the port is good enough
    connect(m_serial, &QSerialPort::errorOccurred, this, &StewartPlatform::closeSerialPort);

    // Send actuator_positions values over serial
    connect(ui->button_send, &QPushButton::clicked, this, [=](){this->SendActuatorPositions(this->actuator_positions);});
}

StewartPlatform::~StewartPlatform()
{
    delete ui;
    delete m_settings;

    closeSerialPort();
    delete m_serial;
}

void StewartPlatform::on_actionExit_triggered()
{
    QApplication::quit();
}

void StewartPlatform::Log(const QString &entry)
{
    ui->log->insertPlainText(entry + "\n");
}

void StewartPlatform::SendActuatorPositions(QVector<int> actuator_pos)
{
    QString s = "";
    for(int i = 0; i < NUM_ACTUATORS; ++i)
    {
        // Convert each actuator int to string, add space if not last element, else new line
        s += QString::number(actuator_pos[i]) + ((i < (NUM_ACTUATORS - 1)) ? " " : "\n");
    }
    writeSerialData(qPrintable(s));
}

void StewartPlatform::readSerialData()
{
    Q_ASSERT(m_serial->isOpen());
    ui->log->insertPlainText(m_serial->readAll());
}

void StewartPlatform::writeSerialData(const char* data)
{
    Q_ASSERT(m_serial->isOpen());
    m_serial->write(data);
}

void StewartPlatform::openSerialPort()
{
    closeSerialPort();
    const SerialSettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite))
    {
        // Update status, enable send button
        ui->label_serial_val->setText(tr("%1").arg(p.name));
        ui->button_send->setEnabled(true);

        Log(tr("Connected to %1 : %2, %3, %4, %5, %6")
                      .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                      .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    }
    else
    {
        QMessageBox::critical(this, tr("Serial Connection Error"), m_serial->errorString());
    }
}

void StewartPlatform::closeSerialPort()
{
    if (m_serial->isOpen())
    {
        m_serial->close();
        Log(tr("Disconnected from %1").arg(m_serial->portName()));

        // Update status, disable send button
        ui->label_serial_val->setText(tr("Disconnected"));
        ui->button_send->setEnabled(false);
    }
}
