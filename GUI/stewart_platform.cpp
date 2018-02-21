#include "stewart_platform.h"

StewartPlatform::StewartPlatform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StewartPlatform),
    m_settings(new SerialSettingsDialog),
    m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    connect(m_serial, &QSerialPort::readyRead, this, &StewartPlatform::readSerialData);
    connect(ui->actionSelect_COM_port, &QAction::triggered, m_settings, &SerialSettingsDialog::show);
    connect(m_settings, &SerialSettingsDialog::SettingsUpdated, this, &StewartPlatform::openSerialPort);
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
    if (m_serial->open(QIODevice::ReadWrite)) {

        Log(tr("Connected to %1 : %2, %3, %4, %5, %6")
                      .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                      .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Serial Connection Error"), m_serial->errorString());

        //showStatusMessage(tr("Open error"));
    }
}

void StewartPlatform::closeSerialPort()
{
    if (m_serial->isOpen())
    {
        m_serial->close();
        Log(tr("Disconnected from %1").arg(m_serial->portName()));
    }
}
