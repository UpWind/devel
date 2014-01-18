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
    ui->pushButton_startTest->setEnabled(true);

}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::setReader(serialPort* read){
    reader=read;
}

void SettingsUI::isSerialPortConnected(bool con){
    if(con)
        serialPortConnected();
    else
        serialPortNotConnected();
}

void SettingsUI::loadSettings(){
    /** Device settings
      */

    portNames = serialPort::getPortNames();

    ui->comboBox_portName->addItems(portNames);

    int k = 600;
    for(int i = 0; i < 7; i++){
        baudRates.append(QString::number(k));
        k *= 2;
    }
    ui->comboBox_baudRate->addItems(baudRates);

    parityList.append("0");
    parityList.append("1");
    ui->comboBox_parity->addItems(parityList);

    stopBitsList.append("1");
    stopBitsList.append("2");
    ui->comboBox_stopBits->addItems(stopBitsList);


    byteSizeList.append("7");
    byteSizeList.append("8");
    ui->comboBox_byteSize->addItems(byteSizeList);

}

void SettingsUI::openPort(){
    if(ui->pushButton_connect->text().compare("Connect")==0){
        ui->pushButton_connect->setText("Disconnect");
        connect(reader, SIGNAL(newNMEAString(const QString &)), this, SLOT(readData(const QString &)));
        emit canRead();
        setSettings();
        if(reader->getConnected()){
            ui->connection_status->setText("<font color =green size+=3><b>Connected to port "
                                           +ui->comboBox_portName->currentText()+"</b></font>");
        }

        else{
            if(portNames.size()==0){
                ui->connection_status->setText("<font color =red size+=3><b>NO PORTS AVAILABLE</b></font>");
            }
            else{
                ui->connection_status->setText("<font color =red size+=3><b>Cannot connect to port "
                                               +ui->comboBox_portName->currentText()+"</b></font>");
            }
        }
    }
    else{
        closePort();
    }
}

bool SettingsUI::readWhenStarted(){
    return ui->checkBox_autoconnect->isChecked();
}

void canRead(){

}

void SettingsUI::readData(const QString & data) {
    //Here we keep the scroll bar at the bottom of the text area
    ui->textEdit->append(data);
    QScrollBar* scroll = ui->textEdit->verticalScrollBar();
    scroll->setValue(scroll->maximum());
}

void SettingsUI::startReading() {
    setSettings();
    reader->setDisplay(true);
}

void SettingsUI::endTest(){
    reader->setDisplay(false);
}

//DefaultSettings loader
void SettingsUI::serialPortConnected(){
    if(ui->checkBox_autoconnect->isChecked() && portNames.size()>0){
        setSettings();
        ui->connection_status->setText("<font color =green size+=3><b>Connected to port "
                                       +ui->comboBox_portName->currentText()+"</b></font>");
        connect(reader, SIGNAL(newNMEAString(const QString &)), this, SLOT(readData(const QString &)));
        ui->pushButton_connect->setText("Disconnect");
    }
    else{
        ui->connection_status->setText("<font color =red size+=3><b>Not Connected</b></font>");
        if(portNames.size()==0){
            ui->connection_status->setText("<font color =red size+=3><b>NO PORTS AVAILABLE</b></font>");
        }
    }

}

void SettingsUI::closePort(){

        disconnect(reader, SIGNAL(newNMEAString(const QString &)), this, SLOT(readData(const QString &)));
        ui->pushButton_connect->setText("Connect");
        ui->connection_status->setText("<font size+=3><b>Disconnected from port "
                              +ui->comboBox_portName->currentText()+"</b></font>");
        emit stopReading();
}

void stopReading(){

}

void SettingsUI::serialPortNotConnected(){

    ui->connection_status->setText("<font color=red size+=3>NOT CONNECTED. Cannot open serial port. Check the connection</font>");
    ui->checkBox_autoconnect->setEnabled(false);
    ui->pushButton_connect->setEnabled(false);
    ui->pushButton_startTest->setEnabled(false);
    ui->comboBox_baudRate->setEnabled(false);
    ui->comboBox_byteSize->setEnabled(false);
    ui->comboBox_parity->setEnabled(false);
    ui->comboBox_portName->setEnabled(false);
    ui->comboBox_stopBits->setEnabled(false);
}

void SettingsUI::setupSettings(Settings *s){
    settings = s;

//    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
//    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
//    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
//    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
//    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
//    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    QComboBox* port = this->findChild<QComboBox*>("comboBox_portName");
    QComboBox* baud = this->findChild<QComboBox*>("comboBox_baudRate");
    QComboBox* parity = this->findChild<QComboBox*>("comboBox_parity");
    QComboBox* stopBits = this->findChild<QComboBox*>("comboBox_stopBits");
    QComboBox* byteSize = this->findChild<QComboBox*>("comboBox_byteSize");
    QCheckBox* autoC = this->findChild<QCheckBox*>("checkBox_autoconnect");

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
//    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
//    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
//    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
//    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
//    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
//    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    QComboBox* port = this->findChild<QComboBox*>("comboBox_portName");
    QComboBox* baud = this->findChild<QComboBox*>("comboBox_baudRate");
    QComboBox* parity = this->findChild<QComboBox*>("comboBox_parity");
    QComboBox* stopBits = this->findChild<QComboBox*>("comboBox_stopBits");
    QComboBox* byteSize = this->findChild<QComboBox*>("comboBox_byteSize");
    QCheckBox* autoC = this->findChild<QCheckBox*>("checkBox_autoconnect");

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

void SettingsUI::setSettings(){
    reader->setSettings(ui->comboBox_portName->currentText().toStdString(),
                        ui->comboBox_baudRate->currentText().toInt(),
                        ui->comboBox_parity->currentText().toInt(),
                        ui->comboBox_stopBits->currentText().toInt(),
                        ui->comboBox_byteSize->currentText().toInt());
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
