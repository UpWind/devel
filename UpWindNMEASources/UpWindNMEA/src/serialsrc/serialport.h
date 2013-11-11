#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class SerialPort {

    public:

    /**
      * Constructor for SerialPort.
      * @param portNumber The COM port number (default COM3).
      * @param baudRate The serial port baud rate.
      */
    SerialPort(unsigned int portNumber = 2, unsigned int baudRate = 4800);
    ~SerialPort();
    /**
      * Outputs an std string to the serial port.
      * @param The string to be outputted.
      */
    void writeString(const std::string &nmeaString);
    /**
      * Outputs a QString to the serial port.
      * @param The string to be outputted.
      */
    void writeString(const QString &nmeaString);
    /**
      * Outputs a single character/byte to the serial port.
      * @param byte The character.
      */
    void writeByte(unsigned char byte);
    /**
      * Closes the serial port.
      */
    void close();
    /**
      * Checks if the port is open.
      */
    bool isOpen();
    /**
      * Reads input from the serial port. Connect this function to a timer.
      * @return The read string from the serial port.
      */
    QString read();

    private:

    unsigned int portNumber;
    unsigned int baudRate;
    bool open;
    QSerialPort *upwindSerial;
    QSerialPortInfo *upwindSerialInfo;

};

#endif // SERIALPORT_H
