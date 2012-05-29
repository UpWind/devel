#include "settingsui.h"
#include "ui_settingsui.h"
#include "serialport.h"
#include "Device.h"

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
    this->loadSettings();
    connect(ui->pushButton_connect, SIGNAL(clicked()), this, SLOT(openPort()));
    connect(ui->pushButton_startTest, SIGNAL(clicked()), this, SLOT(startReading()));
    connect(ui->pushButton_endTest, SIGNAL(clicked()), this, SLOT(endTest()));
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::setReader(serialPort* read){
    reader=read;
}

void SettingsUI::loadSettings(){
    /** Device settings
      */
#if defined (WIN32)
    portNames = serialPort::getPortNames();
#else
    portNames.append("/dev/ttyS0");
    portNames.append("/dev/ttyS1");
#endif

    ui->comboBox_portName->addItems(portNames);
    Device dev;
    dev.autoconnect = false;
    dev.portName = "COM1";
    dev.baudRate = 9600;
    dev.parity = 1;
    dev.byteSize = 7;
    dev.stopBits = 2;
    if(dev.portName != ""){
        if(portNames.contains(dev.portName))
            ui->comboBox_portName->setCurrentIndex(portNames.indexOf(dev.portName));
        else{
            portNames.append(dev.portName);
            ui->comboBox_portName->addItem(dev.portName);
            ui->comboBox_portName->setCurrentIndex(portNames.indexOf(dev.portName));
        }
    }
    ui->checkBox_autoconnect->setChecked(dev.autoconnect);

    int k = 600;
    for(int i = 0; i < 7; i++){
        baudRates.append(QString::number(k));
        k *= 2;
    }
    ui->comboBox_baudRate->addItems(baudRates);
    ui->comboBox_baudRate->setCurrentIndex(baudRates.indexOf(QString::number(dev.baudRate)));

    parityList.append("0");
    parityList.append("1");
    ui->comboBox_parity->addItems(parityList);
    ui->comboBox_parity->setCurrentIndex(parityList.indexOf(QString::number(dev.parity)));
    stopBitsList.append("1");
    stopBitsList.append("2");
    ui->comboBox_stopBits->addItems(stopBitsList);
    ui->comboBox_stopBits->setCurrentIndex(stopBitsList.indexOf(QString::number(dev.stopBits)));

    byteSizeList.append("7");
    byteSizeList.append("8");
    ui->comboBox_byteSize->addItems(byteSizeList);
    ui->comboBox_byteSize->setCurrentIndex(byteSizeList.indexOf(QString::number(dev.byteSize)));
}

void SettingsUI::openPort(){
    reader->setSettings(ui->comboBox_portName->currentText().toStdString(),
                        ui->comboBox_baudRate->currentText().toInt(),
                        ui->comboBox_parity->currentText().toInt(),
                        ui->comboBox_stopBits->currentText().toInt(),
                        ui->comboBox_byteSize->currentText().toInt());
    connect(reader, SIGNAL(newNMEAString(const QString &)), this, SLOT(readData(const QString &)));
    if(reader->getConnected()){
        ui->pushButton_startTest->setEnabled(true);
        ui->pushButton_endTest->setEnabled(true);
        ui->RadioButton_connected->setChecked(true);
        ui->textEdit->setText("<font size+=3><b>*****   Connected to port "
                              +ui->comboBox_portName->currentText()+"   *****</b></font>");
    } else{
        ui->textEdit->setText("<font color=red size+=3>NOT connected</font>");
        ui->RadioButton_NOTconnected->setChecked(true);
    }
}

void SettingsUI::readData(const QString & data) {
    ui->textEdit->append(data);
}

void SettingsUI::startReading() {
    reader->setDisplay(true);
    if(!reader->isRunning())
        reader->start();
}

void SettingsUI::endTest(){
    reader->setDisplay(false);
}

//DefaultSettings loader

void SettingsUI::setupSettings(Settings *s){
    settings = s;

    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    port->setCurrentIndex(portNames.indexOf(s->getSetting("Port")));
    baud->setCurrentIndex(baudRates.indexOf(s->getSetting("Baud")));
    parity->setCurrentIndex(parityList.indexOf(s->getSetting("Parity")));
    stopBits->setCurrentIndex(stopBitsList.indexOf(s->getSetting("StopBits")));
    byteSize->setCurrentIndex(byteSizeList.indexOf(s->getSetting("ByteSize")));

    if(s->getSetting("AutoConnect") == "true")
        autoC->setChecked(true);
    else
        autoC->setChecked(false);
}

void SettingsUI::updateSettings(){
    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    settings->setSetting("Port", QString::fromStdString(port->currentText().toStdString()));
    settings->setSetting("Baud", QString::fromStdString(baud->currentText().toStdString()));
    settings->setSetting("Parity", QString::fromStdString(parity->currentText().toStdString()));
    settings->setSetting("StopBits", QString::fromStdString(stopBits->currentText().toStdString()));
    settings->setSetting("ByteSize", QString::fromStdString(byteSize->currentText().toStdString()));

    if(autoC->isChecked())
        settings->setSetting("AutoConnect","true");
    else
        settings->setSetting("AutoConnect","false");
}

void SettingsUI::save(){
    updateSettings();
}

void SettingsUI::on_comboBox_portName_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_comboBox_baudRate_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_comboBox_parity_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_comboBox_stopBits_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_comboBox_byteSize_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_checkBox_autoconnect_clicked(){
    this->updateSettings();
}
