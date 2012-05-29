#include <QObject>
#include "settingsui.h"
#include "ui_settingsui.h"
#include "nmealogger.h"

SettingsUI::SettingsUI(NMEALogger *logger ) :
    ui(new Ui::SettingsUI)
{
    ui->setupUi(this);
    this->logger = logger;
}

SettingsUI::~SettingsUI(){
    delete ui;
}

void SettingsUI::on_TimedSavingSB_valueChanged(int interval){
    logger->stopSavelogTimer();
    logger->setSavingInterval(interval);
    logger->startSaveLogTimer();
}

void SettingsUI::on_RegularSavingSB_valueChanged(int n){
    logger->setMaxNMEAEntries(n);
}

void SettingsUI::on_TimedSavingRB_toggled(bool checked){
    if(checked){
        ui->TimedSavingSB->setEnabled(true);
        ui->RegularSavingSB->setEnabled(false);

        QObject::connect(this->logger->getSaveLogTimer(), SIGNAL(timeout()), this->logger, SLOT(logNMEAEntries()));
        this->logger->startSaveLogTimer();
    } else{
        ui->RegularSavingSB->setEnabled(true);
        this->logger->stopSavelogTimer();
        QObject::disconnect(this->logger->getSaveLogTimer(), SIGNAL(timeout()));
    }
}

void SettingsUI::on_RegularSavingRB_toggled(bool checked){
    if (checked){
        this->logger->setTimedSavingOn(false);
        QObject::connect(this->logger, SIGNAL( nEntriesReceived() ), this->logger, SLOT(logNMEAEntries()));
        ui->TimedSavingSB->setEnabled(false);
    } else{
        this->logger->setTimedSavingOn(true);
        ui->TimedSavingSB->setEnabled(true);
        QObject::disconnect(this->logger, SIGNAL(nEntriesReceived()));
    }
}

void SettingsUI::updateSettings(){
    if(ui->RegularSavingRB->isChecked()){
        settings->setSetting("RegularRecording", "true");
        settings->setSetting("TimedRecording", "false");
    }

    if (ui->TimedSavingRB->isChecked()){
        settings->setSetting("RegularRecording", "false");
        settings->setSetting("TimedRecording", "true");
    }

    if(ui->gpsCB->isChecked())
        settings->setSetting("GPS","true");
    else
        settings->setSetting("GPS","false");

    if(ui->anemometerCB->isChecked())
        settings->setSetting("Anenometer","true");
    else
        settings->setSetting("Anenometer","false");

    if(ui->compassCB->isChecked())
        settings->setSetting("Compass","true");
    else
        settings->setSetting("Compass","false");

    if(ui->clockCB->isChecked())
        settings->setSetting("Clock","true");
    else
        settings->setSetting("Clock","false");

    settings->setSetting("TimeInterval", QString::number(ui->TimedSavingSB->value()));
    settings->setSetting("RegularInterval", QString::number(ui->RegularSavingSB->value()));
    settings->saveSettings();
}

void SettingsUI::setupSettings(Settings *s){
    settings = s;

    int timeinterval = 0;
    int regular = 0;

    timeinterval = settings->getSetting("TimeInterval").toInt();
    regular = settings->getSetting("RegularInterval").toInt();

    this->logger->setSavingInterval(timeinterval);
    this->logger->setMaxNMEAEntries(regular);

    if(s->getSetting("GPS") == "true"){
        ui->gpsCB->setChecked(true);
        this->logger->setRecordGPS(true);
    } else{
        ui->gpsCB->setChecked(false);
        this->logger->setRecordGPS(false);
    }

    if(s->getSetting("Anenometer") == "true"){
        ui->anemometerCB->setChecked(true);
        this->logger->setRecordWind(true);
    } else{
        ui->anemometerCB->setChecked(false);
        this->logger->setRecordWind(false);
    }

    if(s->getSetting("Compass") == "true"){
        ui->compassCB->setChecked(true);
        this->logger->setRecordCompass(true);
    } else{
        ui->compassCB->setChecked(false);
        this->logger->setRecordCompass(false);
    }

    if(s->getSetting("Clock") == "true"){
        ui->clockCB->setChecked(true);
        this->logger->setRecordClock(true);
    } else{
        ui->clockCB->setChecked(false);
        this->logger->setRecordClock(false);
    }

    if (s->getSetting("TimedRecording") == "true"){
        ui->TimedSavingRB->setChecked(true);
        this->logger->setTimedSavingOn(true);
    } else{
        ui->TimedSavingRB->setChecked(false);
        this->logger->setTimedSavingOn(false);
    }

    if (s->getSetting("RegularRecording") == "true")
        ui->RegularSavingRB->setChecked(true);
    else
        ui->RegularSavingRB->setChecked(false);

    ui->RegularSavingSB->setValue(regular);
    ui->TimedSavingSB->setValue(timeinterval);
}

void SettingsUI::on_anemometerCB_toggled(bool checked){
    if(checked)
        this->logger->setRecordWind(true);
    else
        this->logger->setRecordWind(false);
}

void SettingsUI::on_gpsCB_toggled(bool checked){
    if(checked)
        this->logger->setRecordGPS(true);
    else
        this->logger->setRecordGPS(false);
}

void SettingsUI::on_clockCB_toggled(bool checked){
    if(checked)
        this->logger->setRecordClock(true);
    else
        this->logger->setRecordClock(false);
}

void SettingsUI::on_compassCB_toggled(bool checked){
    if(checked)
        this->logger->setRecordCompass(true);
    else
        this->logger->setRecordCompass(false);
}
