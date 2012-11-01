#include <QtGui>
#include <QFileInfo>
#include <QDebug>
#include "coreupwindscene.h"
#include "../shared/uwmath.h"


CoreUpWindScene::CoreUpWindScene(){

    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setupSettings(settings);
    this->shortNavigation = ShortNavigation::getInstance();//new ShortNavigation();
    this->route = Route::getInstance();
}

void CoreUpWindScene::setChartObjects(CoreChartProvider* model){
    this->model = model;
    fetchChartObjects();
    //connect(model, SIGNAL(modelChanged()), this, SLOT(fetchChartObjects()));
    //model->setAreaFilter(QRect(20,60,5,5));
}

void CoreUpWindScene::fetchChartObjects() {
    chartObjects = model->getChartObjects();
    this->route->loadChartObjects(chartObjects);
    this->shortNavigation->loadChartObjects(chartObjects);

}

void CoreUpWindScene::initializeSettings(){
    QFileInfo info(this->getName() + ".xml");

    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct a new settings
        settings->setSetting("Something", "false");
        settings->setSetting("SomethingElse", "false");
    }
}

void CoreUpWindScene::setBoat(Boat *boat)
{
    this->boat = boat;
}

CoreUpWindScene::~CoreUpWindScene(){
    delete settingsUI;
    delete settings;
}

Boat* CoreUpWindScene::getBoat(){
    return boat;
}

Route* CoreUpWindScene::getRoute(){
    return route;
}

ShortNavigation* CoreUpWindScene::getShortNavigation(){
    return shortNavigation;
}

QString CoreUpWindScene::getName(){
    return QString("CoreUpWindScene");
}

void CoreUpWindScene::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(settingsUI);
}

Settings * CoreUpWindScene::getSettings(){
    return settings;
}

void CoreUpWindScene::parseNMEAString( const QString & text){
    //$IIRMC,,A,2908.38,N,16438.18,E,0.0,0.0,251210,,*06
    if(text[3] == QChar('R') && text[4] == QChar('M') && text[5] == QChar('C')){
        this->parsedNMEAValues.clear();
        QStringList strList = text.split(",");
        this->parsedNMEAValues.append((QString)strList.at(3));

        if(strList.at(4) == "S"){
            this->parsedNMEAValues.append("S");
            latitude = ((QString)strList.at(3)).toDouble();
        } else{
            this->parsedNMEAValues.append("N");
            latitude = ((QString)strList.at(3)).toDouble();
        }
        this->parsedNMEAValues.append((QString)strList.at(5));
        if(strList.at(6) == "W"){
            this->parsedNMEAValues.append("W");
            longitude = ((QString)strList.at(5)).toDouble();
        } else{
            this->parsedNMEAValues.append("E");
           longitude = ((QString)strList.at(5)).toDouble();
        }

        longitude /= 100;
        latitude /= 100;
        qDebug() << Q_FUNC_INFO << latitude << ", " << longitude;
        this->boat->setGeoPosition(longitude, latitude);
    }

    //$IIHDG,15,,,7.1,W*1c
    if(text[3] == QChar('H') && text[4] == QChar('D') && (text[5] == QChar('G') || text[5] == QChar('T'))){
        QStringList strList = text.split(",");
        float heading = ((QString)strList.at(1)).toFloat();
        this->boat->setHeading(heading );
    }
}
Q_EXPORT_PLUGIN2(coreupwindscene, CoreUpWindScene)
