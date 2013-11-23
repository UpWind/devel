#include "serialport.h"
#include "rs232.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <stdexcept>

#include <QString>
#include <QDebug>

//NOTE: The underlying RS232 interface only accepts COM values from 0 - 15, while the COM ports
//      range from 1 - 16.
SerialPort::SerialPort(unsigned int portNumber, unsigned int baudRate):
    portNumber(portNumber), baudRate(baudRate){


    upwindSerialInfo = new QSerialPortInfo("ttyS0");
    upwindSerial = new QSerialPort();

    upwindSerial->setPort(*upwindSerialInfo);
    upwindSerial->open(QIODevice::ReadWrite);
    upwindSerial->setBaudRate(QSerialPort::Baud4800);
    upwindSerial->setParity(QSerialPort::EvenParity);
    upwindSerial->setStopBits(QSerialPort::TwoStop);
    upwindSerial->setDataBits(QSerialPort::Data7);
    upwindSerial->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << upwindSerial->isOpen();

}

SerialPort::~SerialPort() {

    if(upwindSerial->open(QIODevice::ReadWrite)) {
        upwindSerial->close();
    }

}

//Not in use for sending NMEA sentences through the serial port
void SerialPort::writeByte(unsigned char byte) {
    if(open) {
        SendByte(portNumber, byte);
    }

}

void SerialPort::writeString(const std::string &nmeaString) {
    if(isOpen()) {
        upwindSerial->write((nmeaString + std::string("\r\n")).c_str());
    }

}

////Not in use for sending NMEA sentences through the serial port
void SerialPort::writeString(const QString &nmeaString) {
    writeString(nmeaString.toStdString());

}

void SerialPort::close() {

    if(upwindSerial->isOpen()) {
        upwindSerial->close();
    }
}

bool SerialPort::isOpen() {

    return upwindSerial->isOpen();

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
