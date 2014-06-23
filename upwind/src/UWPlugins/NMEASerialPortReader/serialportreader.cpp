#include "serialportreader.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QMessageBox>

QT_USE_NAMESPACE

SerialPortReader::SerialPortReader()
{
    QStringList serialPortNames = availableSerialPorts();

    serialPortBaudRateOptions["4800"] = QSerialPort::Baud4800;
    serialPortBaudRateOptions["9600"] = QSerialPort::Baud9600;
    serialPortBaudRateOptions["19200"] = QSerialPort::Baud19200;

    serialPortDataBitsOptions["8"] = QSerialPort::Data8;
    serialPortDataBitsOptions["7"] = QSerialPort::Data7;
    serialPortDataBitsOptions["6"] = QSerialPort::Data6;
    serialPortDataBitsOptions["5"] = QSerialPort::Data5;

    serialPortParityOptions["Even"] = QSerialPort::EvenParity;
    serialPortParityOptions["Odd"] = QSerialPort::OddParity;
    serialPortParityOptions["None"] = QSerialPort::NoParity;

    serialPortStopBitsOptions["One"] = QSerialPort::OneStop;
    serialPortStopBitsOptions["Two"] = QSerialPort::TwoStop;

    serialPortSettingsUi = new SettingsUI();
    serialPortSettingsUi->initialize(serialPortNames,
        serialPortBaudRateOptions.keys(),
        serialPortDataBitsOptions.keys(),
        serialPortParityOptions.keys(),
        serialPortStopBitsOptions.keys());

    connect(this, &SerialPortReader::serialPortOpenSuccess,
            serialPortSettingsUi, &SettingsUI::serialPortOpened);

    connect(this, &SerialPortReader::serialPortClosed,
            serialPortSettingsUi, &SettingsUI::serialPortClosed);

    connect(this, &SerialPortReader::serialPortError,
            serialPortSettingsUi, &SettingsUI::serialPortError);

    connect(this, &SerialPortReader::serialPortSettingsAppliedSuccessfully,
            serialPortSettingsUi, &SettingsUI::serialPortSettingsApplied);

    connect(this, &SerialPortReader::newNMEAString,
            serialPortSettingsUi, &SettingsUI::appendDataToLogView);

    connect(serialPortSettingsUi, &SettingsUI::toggleSerialPortRequested,
            this, &SerialPortReader::toggleSerialPort);

    connect(serialPortSettingsUi, &SettingsUI::serialPortSettingsChanged,
            this, &SerialPortReader::applySerialPortSettings);

    connect(&serialPort, &QSerialPort::readyRead,
            this, &SerialPortReader::upwindSerialDataReceived);


    // Signaling of changes in QSerialPort properties
    connect(&serialPort, &QSerialPort::baudRateChanged,
            this, &SerialPortReader::serialPortBaudRateChanged);
    connect(this, &SerialPortReader::baudRateChanged,
            serialPortSettingsUi, &SettingsUI::serialPortBaudRateChanged);

    connect(&serialPort, &QSerialPort::parityChanged,
            this, &SerialPortReader::serialPortParityChanged);
    connect(this, &SerialPortReader::parityChanged,
            serialPortSettingsUi, &SettingsUI::serialPortParityChanged);

    connect(&serialPort, &QSerialPort::stopBitsChanged,
            this, &SerialPortReader::serialPortStopBitsChanged);
    connect(this, &SerialPortReader::stopBitsChanged,
            serialPortSettingsUi, &SettingsUI::serialPortStopBitsChanged);

    connect(&serialPort, &QSerialPort::dataBitsChanged,
            this, &SerialPortReader::serialPortDataBitsChanged);
    connect(this, &SerialPortReader::dataBitsChanged,
            serialPortSettingsUi, &SettingsUI::serialPortDataBitsChanged);


    initializeSettings();
}

void SerialPortReader::serialPortBaudRateChanged(qint32 baudRate,
    QSerialPort::Directions directions)
{
    Q_UNUSED(directions)
    emit baudRateChanged(serialPortBaudRateOptions.key(QSerialPort::BaudRate(baudRate)));
}

void SerialPortReader::serialPortParityChanged(QSerialPort::Parity parity) {
   emit parityChanged(serialPortParityOptions.key(parity));
}

void SerialPortReader::serialPortStopBitsChanged(QSerialPort::StopBits stopBits) {
    emit stopBitsChanged(serialPortStopBitsOptions.key(stopBits));
}

void SerialPortReader::serialPortDataBitsChanged(QSerialPort::DataBits dataBits) {
    emit dataBitsChanged(serialPortDataBitsOptions.key(dataBits));
}

void SerialPortReader::applySerialPortSettings(
    const QString &portName,
    const QString &baudRate,
    const QString &dataBits,
    const QString &parity,
    const QString &stopBits)
{
    if (serialPort.setBaudRate(serialPortBaudRateOptions.value(baudRate)) &&
        serialPort.setDataBits(serialPortDataBitsOptions.value(dataBits)) &&
        serialPort.setParity(serialPortParityOptions.value(parity)) &&
        serialPort.setStopBits(serialPortStopBitsOptions.value(stopBits)))
    {
        emit serialPortSettingsAppliedSuccessfully();
    } else {
        emit serialPortError(serialPort.errorString());
    }

    if (!serialPort.isOpen())
        emit serialPortClosed();

    qDebug() << "New serial port settings:"
                << "name: " << serialPort.portName()
                << "bauds: " << serialPort.baudRate()
                   << "databits: " << serialPort.dataBits()
                      << "parity: " << serialPort.parity()
                         << "stop bits: " << serialPort.stopBits();
}

void SerialPortReader::toggleSerialPort(const QString &portName) {
    if (!serialPort.isOpen()) {
        openSerialPort(portName);
    } else {
        closeSerialPort();
    }
}

void SerialPortReader::openSerialPort(const QString &portName) {
    qDebug() << Q_FUNC_INFO << " : " << portName;
    serialPort.setPortName(portName);
    serialPort.open(QIODevice::ReadOnly);
    if (serialPort.error() != QSerialPort::NoError) {
        emit serialPortError(serialPort.errorString());
        emit serialPortClosed();
    } else {
        emit serialPortOpenSuccess();

        serialPortBaudRateChanged(serialPort.baudRate(), QSerialPort::AllDirections);
        serialPortParityChanged(serialPort.parity());
        serialPortStopBitsChanged(serialPort.stopBits());
        serialPortDataBitsChanged(serialPort.dataBits());
    }
}

void SerialPortReader::closeSerialPort() {
    if (serialPort.isOpen()) {
        serialPort.flush();
        serialPort.close();
        emit serialPortClosed();
    } else {
        emit serialPortError("Serial port is already closed.");
    }
}

void SerialPortReader::stopRead(){
    serialPort.close();
}

void SerialPortReader::upwindSerialDataReceived()
{
    if (serialPort.canReadLine()) {
        QString nmeaSentence = QString::fromUtf8(serialPort.readLine());
        if(nmeaSentence.isSimpleText()) {
            emit newNMEAString(nmeaSentence);
        }
    }
}

SerialPortReader::~SerialPortReader(){
    if(serialPort.isOpen())
        serialPort.close();
}

QString SerialPortReader::getName(){
    return "SerialPortReader";
}

const QStringList SerialPortReader::availableSerialPorts() {

    QStringList portNames;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portNames.append(info.portName());
    }
    return portNames;
}

void SerialPortReader::parse(QString line){

    QStringList parsedLine;
    parsedLine = line.split(",");

    // TODO: we could also check the parsedLine[0] ...
    if (parsedLine[0] == "$GPRMC") {
        setRMC(parsedLine);
    } else if (parsedLine[0] == "$IIMWV") {
        setMWV(parsedLine);
    } else if (parsedLine[0] == "$IIVWT") {
        setVWT(parsedLine);
    } else if (parsedLine[0] == "$IIHDT") {
        setHDT(parsedLine);
    } else if (parsedLine[0] == "$IIHD") {
        setHDM(parsedLine);
    } else if (parsedLine[0] == "$IIVHW") {
        setVHW(parsedLine);
    } else if (parsedLine[0] == "$IIDPT") {
        setDPT(parsedLine);
    }
}

//Settings
void SerialPortReader::initializeSettings(){

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

void SerialPortReader::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(serialPortSettingsUi);
}
