#ifndef SERIALPORT_H
#define SERIALPORT_H

#if defined(WIN32)
#include <windows.h>
#include <iostream>
#include <sstream>

#else
typedef unsigned long      DWORD;
typedef unsigned char      BYTE;
typedef int                HANDLE;
#endif
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <string>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QStringList>

#include "../NMEAReader/corenmeareader.h"
#include "settingsui.h"

using namespace std;

/** The serialPort is used handle all the data provides from the Ports,
  * it open the ports, create the connection or close it. Also emit
  * the NMEAStrings
  */
class serialPort: public CoreNMEAReader
{
    Q_OBJECT
public:
    /** Creates the file and timer and initalizate the UI Settings
      */
    serialPort();

    /** Close the connection
      */
    ~serialPort();

    /** Return the name of the plugin
      */
    QString getName() ;

    /** Start or restart the thread reading. Close the connection and it is reopen
      */
    void run();

    /** Setup the display bool
      * @param bool - dictates wether the NMEA sentences should be displayed or not on the screen
      */
    void setDisplay(bool);

    /** Set the new Settings of the port
      * @param portName - Name of the port to connect (e.g. "COM1")
      * @param dwBaudRate - The Baud rate of the connection
      * @param byParity - Parity of data
      * @param byStopBits - StopBits of data
      * @param byByteSize - Byte Size of data
     */
    void setSettings(string, DWORD, BYTE, BYTE, BYTE);

    /**
      Try to connect if is not connected
      @return if finally get the connection
      */
    bool getConnected();

    /** Add the SettingsUI to one layout.
      * @param layout - layout from the Core program
      */
    void addPluginSettingsToLayout(QLayout *layout);

    /** Get the names of the Ports connected to the computer
      * @return a QStringList with all the ports
      */
    static QStringList getPortNames();

    /** Put the settings from a XML file, if  the XML file doesn't exists create new one with the default settings
      */
    void initializeSettings();

private:
    bool display; // true -> dataRead emitted every time a line of data is read
    bool debug;
    HANDLE hComm;
    string portName;
    DWORD dwBaudRate;
    BYTE byParity;
    BYTE byStopBits;
    BYTE byByteSize;

    QStringList RMC;
    QStringList MWV;
    QStringList VWT;
    QStringList HDT;
    QStringList HDM;
    QStringList VHW;
    QStringList DPT;

    bool connected;
    QSerialPort *file;
    QSerialPort serial;

    QSerialPort *upwindSerial;
    QSerialPortInfo *upwindSerialInfo;
    qint8 counter;
    bool opened;

    void startReading();

#if defined(WIN32)
    /** Try to open the actual Port name
      * @return If could be opened
      */
    bool openPort();
    /** Read a line from the HANDLE
      */
    string readLine(DWORD *);

    /** Close the Handle
      */
    void closeConnection();
#endif
    /** Parse and store the NEMAStrings
      * @param line - String to be parsed.
      */
    void parse(QString);

    QTimer* timer;
    int retryCount;

    //settings
    SettingsUI* settingsUI;

private slots:
    void retry();
    void upwindSerialDataReceived();
    void read();
    void stopRead();

signals:
    void notConnected();
    void reconnected();
};

#endif // SERIALPORT_H
