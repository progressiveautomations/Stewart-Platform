#ifndef SERIAL_PORTS_H
#define SERIAL_PORTS_H

#include <QSerialPort>
#include <QSerialPortInfo>

namespace SERIAL_PORTS_H
{
// Currently selected before
static QSerialPort selectedPort;

void openSerialPort(int baud = 9600)
{

}

QList availableSerialPorts()
{
    return QSerialPortInfo::availablePorts();
}

}
#endif // SERIAL_PORTS_H
