#include "settingsui.h"
#include "ui_settingsui.h"
#include "serialportreader.h"
#include "Device.h"
#include <QScrollBar>

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);

    //loadSettings();

    connect(ui->pushButton_openSerialPort, &QPushButton::clicked,
            this, &SettingsUI::toggleSerialPortConnection);

    connect(ui->pushButton_applyPortSettings, &QPushButton::clicked,
            this, &SettingsUI::applySerialPortSettings);

    ui->pushButton_applyPortSettings->setEnabled(true);
    ui->pushButton_openSerialPort->setText("Open");
}

void SettingsUI::initialize(const QStringList &serialPortNames,
                            const QStringList &baudRates,
                            const QStringList &dataBitsOptions,
                            const QStringList &parityOptions,
                            const QStringList &stopBitsOptions)
{
    ui->comboBox_portName->addItems(serialPortNames);
    ui->comboBox_baudRate->addItems(baudRates);
    ui->comboBox_dataBits->addItems(dataBitsOptions);
    ui->comboBox_parity->addItems(parityOptions);
    ui->comboBox_stopBits->addItems(stopBitsOptions);
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::toggleSerialPortConnection() {
    emit toggleSerialPortRequested(ui->comboBox_portName->currentText());
}

void SettingsUI::appendDataToLogView(const QString & data) {
    //Here we keep the scroll bar at the bottom of the text area
    ui->textEdit_logView->append(data);
    const int scrollAreaMaxHeight = ui->textEdit_logView->verticalScrollBar()->maximum();
    ui->textEdit_logView->verticalScrollBar()->setValue(scrollAreaMaxHeight);
}

void SettingsUI::serialPortBaudRateChanged(const QString &baudRate) {
    qDebug() << Q_FUNC_INFO << " : " << baudRate;
    ui->comboBox_baudRate->setCurrentText(baudRate);
}

void SettingsUI::serialPortParityChanged(const QString &parity) {
    qDebug() << Q_FUNC_INFO << " : " << parity;
    ui->comboBox_parity->setCurrentText(parity);
}

void SettingsUI::serialPortStopBitsChanged(const QString &stopBits) {
    qDebug() << Q_FUNC_INFO << " : " << stopBits;
    ui->comboBox_stopBits->setCurrentText(stopBits);
}

void SettingsUI::serialPortDataBitsChanged(const QString &dataBits) {
    qDebug() << Q_FUNC_INFO << " : " << dataBits;
    ui->comboBox_dataBits->setCurrentText(dataBits);
}

void SettingsUI::applySerialPortSettings() {
    emit serialPortSettingsChanged(
        ui->comboBox_portName->currentText(),
        ui->comboBox_baudRate->currentText(),
        ui->comboBox_dataBits->currentText(),
        ui->comboBox_parity->currentText(),
        ui->comboBox_stopBits->currentText());
}

void SettingsUI::serialPortOpened() {
    QString statusText =
        "<font color=green size+=3><b>Serial port open.</b></font>";
    ui->connection_status->setText(statusText);
    ui->pushButton_openSerialPort->setText("Close");
    ui->comboBox_portName->setEnabled(false);
}

void SettingsUI::serialPortError(const QString &errorMsg) {
    appendDataToLogView(errorMsg);
}

void SettingsUI::serialPortClosed() {
    ui->pushButton_openSerialPort->setText("Open");
    ui->connection_status->setText("<font size+=3><b>Serial port is closed. </b></font>");
    ui->comboBox_portName->setEnabled(true);
}

void SettingsUI::serialPortSettingsApplied() {
    appendDataToLogView("Successfully applied settings!");
}

void SettingsUI::serialPortNotConnected(){

    ui->connection_status->setText("<font color=red size+=3>NOT CONNECTED. Cannot open serial port. Check the connection</font>");
    ui->checkBox_autoconnect->setEnabled(false);
    ui->pushButton_openSerialPort->setEnabled(false);
    ui->pushButton_applyPortSettings->setEnabled(false);
    ui->comboBox_baudRate->setEnabled(false);
    ui->comboBox_dataBits->setEnabled(false);
    ui->comboBox_parity->setEnabled(false);
    ui->comboBox_portName->setEnabled(false);
    ui->comboBox_stopBits->setEnabled(false);
}

void SettingsUI::setupSettings(Settings *s){
    //settings = s;

//    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
//    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
//    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
//    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
//    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
//    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    /*
    QComboBox* port = this->findChild<QComboBox*>("comboBox_portName");
    QComboBox* baud = this->findChild<QComboBox*>("comboBox_baudRate");
    QComboBox* parity = this->findChild<QComboBox*>("comboBox_parity");
    QComboBox* stopBits = this->findChild<QComboBox*>("comboBox_stopBits");
    QComboBox* byteSize = this->findChild<QComboBox*>("comboBox_dataBits");
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
        */
}

void SettingsUI::updateSettings(){
//    QComboBox* port= qFindChild<QComboBox*>(this, "comboBox_portName");
//    QComboBox* baud= qFindChild<QComboBox*>(this, "comboBox_baudRate");
//    QComboBox* parity= qFindChild<QComboBox*>(this, "comboBox_parity");
//    QComboBox* stopBits= qFindChild<QComboBox*>(this, "comboBox_stopBits");
//    QComboBox* byteSize= qFindChild<QComboBox*>(this, "comboBox_byteSize");
//    QCheckBox* autoC = qFindChild<QCheckBox*>(this, "checkBox_autoconnect");

    /*
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
        */
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

void SettingsUI::on_comboBox_dataBits_activated(QString){
    this->updateSettings();
}

void SettingsUI::on_checkBox_autoconnect_clicked(){
    this->updateSettings();
}
