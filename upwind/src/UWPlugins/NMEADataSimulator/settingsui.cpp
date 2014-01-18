#include "settingsui.h"
#include "ui_settingsui.h"
#include "datasimulator.h"

SettingsUI::SettingsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
    connect(ui->pushButton_startTest, SIGNAL(clicked()), this, SLOT(startReading()));
    connect(ui->pushButton_endTest, SIGNAL(clicked()), this, SLOT(endTest()));

    first = true;
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::startReading(){
    ui->pushButton_endTest->setEnabled(true);
    ui->pushButton_startTest->setEnabled(false);
    if(first){
        first = false;
        connect(reader, SIGNAL(newNMEAString(const QString &)), this, SLOT(readData(const QString &)));
    }
    reader->setDisplay(true);
    if(!reader->isRunning())
        reader->start();
}
void SettingsUI::endTest(){
    ui->pushButton_endTest->setEnabled(false);
    ui->pushButton_startTest->setEnabled(true);
    reader->setDisplay(false);
    reader->terminate();
}

bool SettingsUI::getGPS(){
    //QCheckBox* gps = qFindChild<QCheckBox*>(this, "checkBox_GPS");
    QCheckBox* gps = this->findChild<QCheckBox*>("checkBox_GPS");
    return gps->isChecked();
}
bool SettingsUI::getAnenomter(){
    //QCheckBox* an = qFindChild<QCheckBox*>(this, "checkBox_Anenometer");
    QCheckBox* an = this->findChild<QCheckBox*>("checkBox_Anenometer");
    return an->isChecked();
}
bool SettingsUI::getCompass(){
    //QCheckBox* com = qFindChild<QCheckBox*>(this, "checkBox_Compass");
    QCheckBox* com = this->findChild<QCheckBox*>("checkBox_Compass");
    return com->isChecked();
}

double SettingsUI::boatPositionLongitude()
{
    return ui->boatPosLonDSpinBox->value();
}

double SettingsUI::boatPositionLatitude()
{
    return ui->boatPosLatDSpinBox->value();
}

bool SettingsUI::getClock(){
    //QCheckBox* clo = qFindChild<QCheckBox*>(this, "checkBox_Clock");
    QCheckBox* clo = this->findChild<QCheckBox*>("checkBox_Clock");
    return clo->isChecked();
}

//DefaultSettings loader
void SettingsUI::setupSettings(Settings *s){
    settings = s;

    if(s->getSetting("GPS") == "true")
        ui->checkBox_GPS->setChecked(true);
    else
        ui->checkBox_GPS->setChecked(false);

    if(s->getSetting("Anenometer") == "true")
        ui->checkBox_Anenometer->setChecked(true);
    else
        ui->checkBox_Anenometer->setChecked(false);

    if(s->getSetting("Compass") == "true")
        ui->checkBox_Compass->setChecked(true);
    else
        ui->checkBox_Compass->setChecked(false);

    if(s->getSetting("Clock") == "true")
        ui->checkBox_Clock->setChecked(true);
    else
        ui->checkBox_Clock->setChecked(false);

    double boatPosLon = settings->getSetting("boatPosLongitude").toDouble();
    double boatPosLat =  settings->getSetting("boatPosLatitude").toDouble();
    int time = s->getSetting("Timer").toInt();

    ui->boatPosLonDSpinBox->setValue(boatPosLon);
    ui->boatPosLatDSpinBox->setValue(boatPosLat);
    ui->horizontalSlider_velocity->setValue(time);
    changeSpeed(time);
}

void SettingsUI::updateSettings(){
    qDebug() << Q_FUNC_INFO;
    if(ui->checkBox_GPS->isChecked())
        settings->setSetting("GPS", "true");
    else
        settings->setSetting("GPS", "false");

    if(ui->checkBox_Anenometer->isChecked())
        settings->setSetting("Anenometer", "true");
    else
        settings->setSetting("Anenometer", "false");

    if(ui->checkBox_Compass->isChecked())
        settings->setSetting("Compass", "true");
    else
        settings->setSetting("Compass", "false");

    if(ui->checkBox_Clock->isChecked())
        settings->setSetting("Clock", "true");
    else
        settings->setSetting("Clock", "false");

    settings->setSetting("Timer", QString::number(ui->horizontalSlider_velocity->value()));
    settings->setSetting("boatPosLongitude", QString::number(ui->boatPosLonDSpinBox->value()));
    settings->setSetting("boatPosLatitude", QString::number(ui->boatPosLatDSpinBox->value()));
}

void SettingsUI::setReader(dataSimulator* read){
    reader = read;
    connect(ui->checkBox_GPS, SIGNAL (clicked()), reader, SLOT(changeGPS()));
    connect(ui->checkBox_Anenometer, SIGNAL (clicked()), reader, SLOT(changeAnenometer()));
    connect(ui->checkBox_Clock, SIGNAL (clicked()), reader, SLOT(changeClock()));
    connect(ui->checkBox_Compass, SIGNAL (clicked()), reader, SLOT(changeCompass()));
    connect(ui->horizontalSlider_velocity, SIGNAL(valueChanged(int)), reader, SLOT(changeSpeed(int)));
    connect(ui->horizontalSlider_velocity, SIGNAL(valueChanged(int)), this, SLOT(changeSpeed(int)));
    connect(ui->boatPosLonDSpinBox, SIGNAL(valueChanged(double)), reader, SLOT(setBoatPositionLon(double)));
    connect(ui->boatPosLatDSpinBox, SIGNAL(valueChanged(double)), reader, SLOT(setBoatPositionLat(double)));
}

void SettingsUI::changeSpeed(int s){
    float ms = 60 * s;
    ms = ms / 1000;
    ui->speedSeg->setText(QString::number(ms) + " seg");
}

void SettingsUI::save(){
    updateSettings();
}

void SettingsUI::readData(const QString & data){
    if(reader->getDisplay()) ui->textEdit->append(data);
}

void SettingsUI::on_horizontalSlider_velocity_valueChanged(int value){
	(void)value;
    this->updateSettings();
}

void SettingsUI::on_boatPosLonDSpinBox_valueChanged(double val) {
    this->updateSettings();
}

void SettingsUI::on_boatPosLatDSpinBox_valueChanged(double val) {
    this->updateSettings();
}

void SettingsUI::on_checkBox_GPS_clicked(){
    this->updateSettings();
}

void SettingsUI::on_checkBox_Anenometer_clicked(){
    this->updateSettings();
}

void SettingsUI::on_checkBox_Compass_clicked(){
    this->updateSettings();
}

void SettingsUI::on_checkBox_Clock_clicked(){
    this->updateSettings();
}
