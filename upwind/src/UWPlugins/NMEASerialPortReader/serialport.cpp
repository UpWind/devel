#include "serialport.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QMessageBox>

QT_USE_NAMESPACE

serialPort::serialPort()
{
    upwindSerialInfo = new QSerialPortInfo("ttyS0");
    upwindSerial = new QSerialPort();

    upwindSerial->setPort(*upwindSerialInfo);
    upwindSerial->open(QIODevice::ReadWrite);
    // if !upwindSerial.isOpen
    // //Do not show SerialPortReader tab in the settings
    upwindSerial->setBaudRate(QSerialPort::Baud4800);
    upwindSerial->setParity(QSerialPort::EvenParity);
    upwindSerial->setStopBits(QSerialPort::TwoStop);
    upwindSerial->setDataBits(QSerialPort::Data7);
    upwindSerial->setFlowControl(QSerialPort::NoFlowControl);

    connect(upwindSerial,SIGNAL(readyRead()),this,SLOT(upwindSerialDataReceived()));
    //for the connect button in the UI
    //disconnect(upwindSerial,SIGNAL(readyRead()),this,SLOT(upwindSerialDataReceived()));

    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setReader(this);
    settingsUI->setupSettings(settings);
}
/*
serialPort::serialPort(){

#ifndef WIN32
    //file = new QSerialPort(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
            qDebug() << "Port opened correctly. This is a debugging message";
        else
            qDebug() << "ESTO NO VAAA!";

    }

#endif
    display = false;
    connected = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(retry()));

//    portName = "COM1";
    portName = "/dev/ttyS0";




#ifndef WIN32

    serial.setPortName("/dev/ttyS0");

    qDebug() << "Crash before open mode";
    qDebug() << serial.setBaudRate(QSerialPort::Baud9600);
    qDebug() << "After setting baud rate";
    qDebug() << serial.setParity(QSerialPort::OddParity);
    qDebug() << serial.setStopBits(QSerialPort::TwoStop);
    qDebug() << serial.setDataBits(QSerialPort::Data8);

#endif

    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setReader(this);
    settingsUI->setupSettings(settings);

}
*/
void serialPort::upwindSerialDataReceived()
{

    if (upwindSerial->canReadLine()){
        QString cadena=QString::fromUtf8(upwindSerial->readLine());
        qDebug() << "Data received from the serial port: " << cadena;
        emit newNMEAString(cadena);
    }
}
serialPort::~serialPort(){
#if defined(WIN32)
    if(connected)
        this->closeConnection();
#else
    if(connected)
        serial.close();
#endif
    connected = false;
}

QString serialPort::getName(){
    QString name= "SerialPortReader";
    return name;
}

void serialPort::setSettings(string _portName, DWORD _dwBaudRate, BYTE _byParity, BYTE _byStopBits, BYTE _byByteSize){
    qDebug() << "Metodo 3";

    portName = _portName;
    dwBaudRate = _dwBaudRate;
    byParity = _byParity;
    byStopBits = _byStopBits;
    byByteSize = _byByteSize;
    qDebug() << "MMAMAMAMAMAMAMAMAM";
}

bool serialPort::getConnected(){

#if defined(WIN32)
    if(!connected)
        if(this->openPort())
            connected = true;
#else
    //    if(!connected)
    //        if(file->open(QFile::ReadOnly))
    connected = true;
#endif
    return connected;
}

void serialPort::setDisplay(bool display){

    this->display = display;
}

void serialPort::retry(){
    emit notConnected();
    this->run();
}

#if defined(WIN32)
bool serialPort::openPort() {
    hComm = CreateFileA(portName.c_str(),
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        0, //FILE_FLAG_OVERLAPPED,
                        0);
    if(hComm == INVALID_HANDLE_VALUE){
        connected = false;
        return false;
    }

    DCB dcb = {0};
    dcb.DCBlength = sizeof(DCB);

    if(!::GetCommState (hComm, &dcb)){
        connected = false;
        return false;
    }
    dcb.BaudRate = dwBaudRate;
    dcb.ByteSize = byByteSize;
    dcb.Parity = byParity;
    if(byStopBits == 1)
        dcb.StopBits = ONESTOPBIT;
    else if(byStopBits == 2)
        dcb.StopBits = TWOSTOPBITS;
    else
        dcb.StopBits = ONE5STOPBITS;
    dcb.fBinary = TRUE;
    dcb.fParity = TRUE;

    if(!::SetCommState (hComm, &dcb)){
        connected = false;
        return false;
    }

    COMMTIMEOUTS commTimeout;
    if(::GetCommTimeouts(hComm, &commTimeout)){
        commTimeout.ReadIntervalTimeout = 10;
        commTimeout.ReadTotalTimeoutConstant = 1;
        commTimeout.ReadTotalTimeoutMultiplier = 1;
        commTimeout.WriteTotalTimeoutConstant = 50;
        commTimeout.WriteTotalTimeoutMultiplier = 10;
    } else{
        connected = false;
        return false;
    }

    if(!SetCommTimeouts(hComm, &commTimeout)){
        connected = false;
        return false;
    }
    connected = true;
    return true;
}
#endif

//    void serialPort::startReading() {
//    #if defined(WIN32)
//        qDebug() << "Starting reading the serial port in Windows";
//        string line;
//        DWORD dwEventMask;
//        bool moreData = false;
//        if(!::SetCommMask(hComm, EV_RXCHAR))
//            return;
//        if(::WaitCommEvent(hComm, &dwEventMask, NULL)){
//            DWORD dwSize = 0;
//            do{
//                line = readLine(&dwSize);
//                if(line != ""){
//                    moreData = true;
//                    this->parse(QString::fromStdString(line));
//                    const QString s = QString::fromStdString(line);
//                    if (display){
//                        qDebug() << "Emiting NMEA sentence";
//                        qDebug() << s;
//                        emit newNMEAString(s);}
//                } else
//                    moreData = false;
//            } while(moreData);

//        }
//        else
//            moreData = false;
//    #else
//        qDebug() << "Starting reading serial port in Linux";
//        if(this->getConnected()){
//            connected = true;

//            char buf[1024];
//            qDebug() << "Print 1";
//            serial.waitForReadyRead(100);
//                    qDebug() << "Print 2";
//            qDebug() << serial.bytesAvailable();
//                    qDebug() << "Print 3";
//                    while(true)
//                        if(serial.bytesAvailable()!=0){
//                            serial.read(buf, sizeof(buf));
//                            //serial.read()
//                            QString line(buf);
//                            this->parse(line);
//                            QString s = line;
//                            qDebug() << s;
//                        }
//    //            qint64 lineLength = file->readLine(buf, sizeof(buf));
//    //            qDebug() << "serial data: " << lineLength;
//    //            if(lineLength != -1){
//    //                connected = true;
//    //                QString line(buf);
//    //                if(line.compare("") != 0){
//    //                    this->parse(line);
//    //                    QString s = line;
//    //                    if(display){
//    //                        //The star marks that the sentence's source is the serial port.
//    //                        if( s[3]==QChar('R')){ // This is a very bad work but since I don't know how to do it better, I will leave this sentence.
//    //                            s.insert(s.length(),"*");
//    //                        }
//    //                        emit newNMEAString(s);
//    //                        this->usleep(300000);
//    //                    }
//    //                    //this->sleep(1); If this line is not commented ther is a big delay (4 or 5 minutes) between the NMEA sentence is sent and the simulator reaction to that NMEA sentence.
//    //                }
//    //            } else
//    //                this->usleep(500000);

//        }
//    #endif
//    }

#if defined(WIN32)
string serialPort::readLine(DWORD *dwSize){
    std::stringbuf sb;
    char szBuf;
    DWORD dwIncommingReadSize;
    bool endLine = false;
    do{
        if(ReadFile(hComm, &szBuf, 1, &dwIncommingReadSize, NULL) != 0){
            if(dwIncommingReadSize > 0){
                if(szBuf != '\n')
                    sb.sputn(&szBuf, dwIncommingReadSize);
                else
                    endLine = true;
            }
        } else
            return sb.str();
    } while(!endLine);
    return sb.str();
}

void serialPort::closeConnection(){
    if(hComm != INVALID_HANDLE_VALUE){
        CloseHandle(hComm);
        hComm = INVALID_HANDLE_VALUE;
    }
}
#endif

void serialPort::run(){
#if defined(WIN32)
    if(connected || this->openPort()){
        retryCount = 0;
        if(timer->isActive())
            timer->stop();
        this->startReading();
        this->closeConnection();
        timer->start(2000);
    }

#else
    this->startReading();
    file->close();
    connected = false;
#endif
}

#if defined(WIN32)
QStringList serialPort::getPortNames(){
    // Getting names of available COM ports
    QStringList portNames;
    char port_name[16];
    for(int port_number = 1; port_number < 16; ++port_number){
        port_number < 10 ? sprintf(port_name, "COM%d", port_number) :
                           sprintf(port_name, "\\\\.\\COM%d", port_number);
        HANDLE hFile = ::CreateFileA(port_name,
                                     GENERIC_READ, 0, NULL,
                                     OPEN_EXISTING, 0, NULL);
        if(hFile != INVALID_HANDLE_VALUE)
            portNames.append(QString::fromAscii(port_name));
        else{
            DWORD err = GetLastError();
            if(err == ERROR_ACCESS_DENIED ||
                    err == ERROR_SHARING_VIOLATION)
                portNames.append(QString::fromAscii(port_name));
        }
        CloseHandle(hFile);
    }
    return portNames;
}
#endif

void serialPort::parse(QString line){
    if(line.startsWith("$GPRMC")){
        RMC = line.split(",");
        setRMC(RMC);
    }else if (line.startsWith("$IIMWV")){
        MWV = line.split(",");
        setMWV(MWV);
    }else if (line.startsWith("$IIVWT")){
        VWT = line.split(",");
        setVWT(VWT);
    }else if (line.startsWith("$IIHDT")){
        HDT = line.split(",");
        setHDT(HDT);
    }else if (line.startsWith("$IIHD")){
        HDM = line.split(",");
        setHDM(HDM);
    }else if (line.startsWith("$IIVHW")){
        VHW = line.split(",");
        setVHW(VHW);
    }else if (line.startsWith("$IIDPT")){
        DPT = line.split(",");
        setDPT(DPT);
    }
}

//Settings
void serialPort::initializeSettings(){
    QFileInfo info(this->getName() + ".xml");

    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct a new settings
        settings->setSetting("Port", "COM1");
        settings->setSetting("Baud", "9600");
        settings->setSetting("Parity", "1");
        settings->setSetting("StopBits", "2");
        settings->setSetting("ByteSize", "7");
        settings->setSetting("AutoConnect", "true");
    }
}

void serialPort::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(settingsUI);
}

Q_EXPORT_PLUGIN2(serialport, serialPort)
