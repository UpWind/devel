#include "pgdatabase.h"
#include "layers/LayersManager.h"
#include <QDebug>

PGDataBase::PGDataBase(){
    qDebug() << Q_FUNC_INFO;
    settingsUI = new SettingsUI();
    settingsUI->setChart(this);
    this->initializeSettings();

    settingsUI->setupSettings(settings);
    layersManager = LayersManager::getInstance();
}

PGDataBase::~PGDataBase(){}

void PGDataBase::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(settingsUI);
}

QString PGDataBase::getName(){
    return QString("PostgreDataBase");
}

QString PGDataBase::getDBName(){
    return DBName;
}

QString PGDataBase::getDBUser(){
    return DBUser;
}

QString PGDataBase::getDBPass(){
    return DBPass;
}

QString PGDataBase::getDBPort(){
    return DBPort;
}

QString PGDataBase::getDBHost(){
    return DBHost;
}

QString PGDataBase::getDBDriver(){
    return DBDriver;
}

void PGDataBase::setDBName(QString string){
    DBName = string;
}

void PGDataBase::setDBUser(QString string){
    DBUser = string;
}

void PGDataBase::setDBPass(QString string){
    DBPass = string;
}

void PGDataBase::setDBPort(QString string){
    DBPort = string;
}

void PGDataBase::setDBHost(QString string){
    DBHost = string;
}

void PGDataBase::setDBDriver(QString string){
    DBDriver = string;
}

void PGDataBase::initializeSettings(){
    QFileInfo info(this->getName() + ".xml");

    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct a new settings
        settings->setSetting("User", "newUser");
        settings->setSetting("Password", "newPass");
        settings->setSetting("Port", "yourPort");
        settings->setSetting("Host", "yourHost");
        settings->setSetting("Driver", "PostgreSQL");
        settings->setSetting("DBName", "");
        settings->setSetting("XML", "");
    }
}

Q_EXPORT_PLUGIN2(pgdatabase, PGDataBase)
