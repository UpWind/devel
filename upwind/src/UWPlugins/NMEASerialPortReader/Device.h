#ifndef DEVICE_H
#define DEVICE_H
#include <QString>

struct Device{
public:
    bool autoconnect;
    QString portName;
    int baudRate;
    int parity;
    int stopBits;
    int byteSize;
};

#endif // DEVICE_H
