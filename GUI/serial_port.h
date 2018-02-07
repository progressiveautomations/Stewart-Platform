#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

// Based on code written by manashmndl on Github.

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <stdlib.h>

class SerialPort
{
public:
    SerialPort(char *port_name);
    ~SerialPort();

    bool isConnected();

    int readSerialPort(char *buf, unsigned int buf_size);
    bool writeSerialPort(char *buf, unsigned int buf_size);

private:
    bool is_connected;

    HANDLE handler;
    COMSTAT status;
    DWORD errors;
};

#endif // SERIAL_PORT_H
