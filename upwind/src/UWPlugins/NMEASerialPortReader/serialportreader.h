#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#if defined(WIN32)
#include <windows.h>
#include <iostream>
#include <sstream>

#else
typedef unsigned long      DWORD;
typedef unsigned char      BYTE;
typedef int                HANDLE;
#endif

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QStringList>

#include "../NMEAReader/corenmeareader.h"
#include "settingsui.h"

using namespace std;

/** The SerialPortReader is used handle the NMEA data from serial port.
  */
class SerialPortReader: public CoreNMEAReader
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.SerialPortReader")

public:
    /** Creates the file and timer and initalizate the UI Settings
      */
    SerialPortReader();

    /** Close the connection
      */
    ~SerialPortReader();

    /** Return the name of the plugin
      */
    QString getName() ;

    /** Add the SettingsUI to one layout.
      * @param layout - layout from the Core program
      */
    void addPluginSettingsToLayout(QLayout *layout);

    /** Put the settings from a XML file, if  the XML file doesn't exists create new one with the default settings
      */
    void initializeSettings();

private:

    /** Get names of available serial ports in the system
      * @return a QStringList with serial port names
      */
    const QStringList availableSerialPorts();

    QSerialPort serialPort;

    /** Parse and store the NEMAStrings
      * @param line - String to be parsed.
      */
    void parse(QString);

    QTimer* timer;
    int retryCount;

    // UI for SerialPortReader settings
    SettingsUI *serialPortSettingsUi;

    /**
     * Opens serial port by portName
     * @param portName
     */
    void openSerialPort(const QString &portName);

    /**
     * Closes opened serial port.
     *
     * Closes the opened serial port and emits serialPortClosed() signal.
     * If serial port is not open does nothing and emits serialPortError() signal.
     */
    void closeSerialPort();

    QHash<QString, QSerialPort::BaudRate> serialPortBaudRateOptions;
    QHash<QString, QSerialPort::Parity> serialPortParityOptions;
    QHash<QString, QSerialPort::DataBits> serialPortDataBitsOptions;
    QHash<QString, QSerialPort::StopBits> serialPortStopBitsOptions;


private slots:
    void upwindSerialDataReceived();
    void toggleSerialPort(const QString &portName);
    void stopRead();
    void applySerialPortSettings(
        const QString &portName,
        const QString &baudRate,
        const QString &dataBits,
        const QString &parity,
        const QString &stopBits);

    void serialPortBaudRateChanged(qint32 baudRate, QSerialPort::Directions directions);
    void serialPortParityChanged(QSerialPort::Parity parity);
    void serialPortStopBitsChanged(QSerialPort::StopBits stopBits);
    void serialPortDataBitsChanged(QSerialPort::DataBits dataBits);

signals:
    void notConnected();
    void reconnected();

    void serialPortError(QString errorMsg);
    void serialPortOpenSuccess();
    void serialPortClosed();
    void serialPortSettingsAppliedSuccessfully();

    void baudRateChanged(const QString &baudRate);
    void parityChanged(const QString &parity);
    void stopBitsChanged(const QString &stopBits);
    void dataBitsChanged(const QString &dataBits);
};

#endif // SERIALPORT_H
