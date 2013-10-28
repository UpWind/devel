#include "serialport.h"

#include <QMessageBox>

serialPort::serialPort(){

#ifndef WIN32
    file = new QFile();
#endif
    display = false;
    connected = false;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(retry()));

//    portName = "COM1";
    portName = "/dev/ttyS0";

#ifndef WIN32
    file->setFileName(QString::fromStdString(portName));
#endif

//#if defined(Q_OS_LINUX)
//    qDebug() << "yes yes its unix";
//#endif


    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setReader(this);
    settingsUI->setupSettings(settings);

}

serialPort::~serialPort(){
#if defined(WIN32)
    if(connected)
        this->closeConnection();
#else
    if(connected)
        file->close();
    delete (file);
#endif
    connected = false;
}

QString serialPort::getName(){
    QString name= "SerialPortReader";
    return name;
}

void serialPort::setSettings(string _portName, DWORD _dwBaudRate, BYTE _byParity, BYTE _byStopBits, BYTE _byByteSize){
    portName = _portName;
    dwBaudRate = _dwBaudRate;
    byParity = _byParity;
    byStopBits = _byStopBits;
    byByteSize = _byByteSize;
#ifndef WIN32
    if(!connected){
        file->setFileName(QString::fromStdString(portName));
    }
#endif
}

bool serialPort::getConnected(){
#if defined(WIN32)
    if(!connected)
        if(this->openPort())
            connected = true;
#else
    if(!connected)
        if(file->open(QFile::ReadOnly))
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

void serialPort::startReading() {
#if defined(WIN32)
    string line;
    DWORD dwEventMask;
    bool moreData = false;
    if(!::SetCommMask(hComm, EV_RXCHAR))
        return;
    if(::WaitCommEvent(hComm, &dwEventMask, NULL)){
        DWORD dwSize = 0;
        do{
            line = readLine(&dwSize);
            if(line != ""){
                moreData = true;
                this->parse(QString::fromStdString(line));
                const QString s = QString::fromStdString(line);
                if (display)
                    emit newNMEAString(s);
            } else
                moreData = false;
        } while(moreData);

    }
    else
        moreData = false;
#else
    bool moreData = false;
    if(this->getConnected()){
        connected = true;
        moreData = true;
        char buf[1024];
        while(moreData){ // A line from the port is read while there is one available.
            qint64 lineLength = file->readLine(buf, sizeof(buf));
            qDebug() << "serial data: " << lineLength;
            if(lineLength != -1){
                connected = true;
                QString line(buf);
                if(line.compare("") != 0){
                    this->parse(line);
                    const QString s = line;
                    if(display)
                        emit newNMEAString(s);
                    this->sleep(1);
                }
                moreData = true;
            } else
                moreData = false;
        }
    }
#endif
}

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
