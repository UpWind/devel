#include "serialport.h"
#include "rs232.h"

#include <stdexcept>

#include <QString>
#include <QDebug>

//NOTE: The underlying RS232 interface only accepts COM values from 0 - 15, while the COM ports
//      range from 1 - 16.
SerialPort::SerialPort(unsigned int portNumber, unsigned int baudRate):
    portNumber(portNumber), baudRate(baudRate){

    int error = OpenComport(portNumber, baudRate);
    this->open = (!error ? true : false);

    if(!open) {
        //throw std::runtime_error("Failed to open serial port!");
        qDebug() << "Failed to open serial port!";
    }

}

SerialPort::~SerialPort() {

    if(open) {
        CloseComport(portNumber);
    }

}

void SerialPort::writeByte(unsigned char byte) {

    if(open) {
        SendByte(portNumber, byte);
    }

}

void SerialPort::writeString(const std::string &nmeaString) {
    if(open) {
        cprintf(portNumber, (nmeaString + std::string("\r\n")).c_str());
    }

}

void SerialPort::writeString(const QString &nmeaString) {

    writeString(nmeaString.toStdString());

}

void SerialPort::close() {

    if(open) {
        CloseComport(portNumber);
    }
}

bool SerialPort::isOpen() {

    return open;

}

QString SerialPort::read() {

    unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char) * 4096);
    int charCount = PollComport(portNumber, buffer, 4096);
    QString msg("Empty port buffer!");
    if(charCount != 0) {
        msg = ((const char *)buffer);
        msg.chop(msg.size() - charCount);
    }
    free(buffer);
    return msg;

}
