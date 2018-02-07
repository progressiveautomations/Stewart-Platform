#include "serial_port.h"

SerialPort::SerialPort(char *port_name)
{
    this->is_connected = false;

    this->handler = CreateFileA
    (
        static_cast<LPCSTR>(port_name),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (this->handler)
}
