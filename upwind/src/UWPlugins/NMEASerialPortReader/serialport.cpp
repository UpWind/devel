#include "serialport.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QMessageBox>

QT_USE_NAMESPACE

serialPort::serialPort()
{
        if (QSerialPortInfo::availablePorts().count() != 0){
            QString port = getPortNames().at(0);
            upwindSerialInfo = new QSerialPortInfo(port);
        }
        else{
            upwindSerialInfo = new QSerialPortInfo("port");
        }


    upwindSerial = new QSerialPort();
    upwindSerial->setPort(*upwindSerialInfo);
    opened = upwindSerial->open(QIODevice::ReadWrite);

    upwindSerial->setBaudRate(QSerialPort::Baud4800);
    upwindSerial->setParity(QSerialPort::EvenParity);
    upwindSerial->setStopBits(QSerialPort::TwoStop);
    upwindSerial->setDataBits(QSerialPort::Data7);
    upwindSerial->setFlowControl(QSerialPort::NoFlowControl);

    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setReader(this);
    settingsUI->setupSettings(settings);
    settingsUI->isSerialPortConnected(opened);

    connect(settingsUI,SIGNAL(canRead()),this,SLOT(read()));
    connect(settingsUI,SIGNAL(stopReading()),this,SLOT(stopRead()));


    if(settingsUI->readWhenStarted()){
        upwindSerial->clear(QSerialPort::AllDirections);
        connect(upwindSerial,SIGNAL(readyRead()),this,SLOT(upwindSerialDataReceived()));
    }


}
void serialPort::read(){
    if(!getConnected()){
        opened = upwindSerial->open(QIODevice::ReadWrite);
    }
    upwindSerial->clear(QSerialPort::AllDirections);
    connect(upwindSerial,SIGNAL(readyRead()),this,SLOT(upwindSerialDataReceived()));



}

void serialPort::stopRead(){
    upwindSerial->close();
}


void serialPort::upwindSerialDataReceived()
{

    if (upwindSerial->canReadLine()){
        QString cadena=QString::fromUtf8(upwindSerial->readLine());
       // qDebug() << "Data received from the serial port: " << cadena;
        if(cadena.isSimpleText()){
            emit newNMEAString(cadena);
        }
    }
}
serialPort::~serialPort(){
    if(getConnected())
        upwindSerial->close();
    connected = false;
}

QString serialPort::getName(){
    QString name= "SerialPortReader";
    return name;
}

void serialPort::setSettings(string _portName, DWORD _dwBaudRate, BYTE _byParity, BYTE _byStopBits, BYTE _byByteSize){


    qDebug() << "Settings have changed";

    portName = _portName;
    dwBaudRate = _dwBaudRate;
    byParity = _byParity;
    byStopBits = _byStopBits;
    byByteSize = _byByteSize;

    //This is for changing the port when the not connect message appears
//    if(!opened){
//        upwindSerial->setPortName(QString::fromStdString(portName));
//        opened = upwindSerial->open(QIODevice::ReadWrite);
//    }

    switch(_byByteSize){
    case 7:{
        upwindSerial->setDataBits(QSerialPort::Data7);
        break;
    }
    case 8:{
        upwindSerial->setDataBits(QSerialPort::Data8);
        break;
    }
    default:
        upwindSerial->setDataBits(QSerialPort::Data7);
    }

    switch(_byStopBits){
    case 1:{
        upwindSerial->setStopBits(QSerialPort::OneStop);
        break;
    }
    case 2:{
        upwindSerial->setStopBits(QSerialPort::TwoStop);
        break;
    }
    default:
        upwindSerial->setStopBits(QSerialPort::TwoStop);
    }

    switch(_byParity){
    case 1:{
        upwindSerial->setParity(QSerialPort::OddParity);
        break;
    }
    case 0:{
        upwindSerial->setParity(QSerialPort::EvenParity);
        break;
    }
    default:
        upwindSerial->setParity(QSerialPort::EvenParity);
    }

    switch(_dwBaudRate){
    case 1200: {
        upwindSerial->setBaudRate(QSerialPort::Baud1200);
        break;
    }
    case 2400: {
        upwindSerial->setBaudRate(QSerialPort::Baud2400);
        break;
    }
    case 4800: {
        upwindSerial->setBaudRate(QSerialPort::Baud4800);
        break;
    }
    case 9600: {
        upwindSerial->setBaudRate(QSerialPort::Baud9600);
        break;
    }
    case 19200: {
        upwindSerial->setBaudRate(QSerialPort::Baud19200);
        break;
    }
    case 38400: {
        upwindSerial->setBaudRate(QSerialPort::Baud38400);
        break;
    }
    case 57600: {
        upwindSerial->setBaudRate(QSerialPort::Baud57600);
        break;
    }
    default:
        upwindSerial->setBaudRate(QSerialPort::Baud4800);
    }

    qDebug() << "New setting are:\n Baud Rate: " << dwBaudRate << "\n Parity:" << byParity << "\n Stop Bits:" << byStopBits << "\n Byte size:" << byByteSize;
}

bool serialPort::getConnected(){

    return upwindSerial->isOpen();
}

void serialPort::setDisplay(bool display){

    this->display = display;
}

void serialPort::retry(){
    emit notConnected();
    this->run();
}

void serialPort::run(){

}


QStringList serialPort::getPortNames(){

    QStringList portNames;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portNames.append(info.portName());
    }
    return portNames;
}


void serialPort::parse(QString line){
    upwindSerialInfo->availablePorts();
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
        settings->setSetting("Baud", "4800");
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
