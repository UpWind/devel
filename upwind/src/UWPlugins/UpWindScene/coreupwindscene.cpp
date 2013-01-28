#include <QtGui>
#include <QFileInfo>
#include <QDebug>
#include "coreupwindscene.h"
#include "../shared/uwmath.h"
#include "Scene/calculatelaylines.h"


CoreUpWindScene::CoreUpWindScene(){

    this->initializeSettings();

    settingsUI = new SettingsUI();
    settingsUI->setupSettings(settings);
    this->loadObstacles = LoadObstacles::getInstance();
    this->route = Route::getInstance();
    this->pDiagram = PolarDiagram::getInstance();
    qRegisterMetaType<QVector<QPointF> >("QVector<QPointF>");
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
    this->loadObstacles->loadChartObjects(chartObjects);

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

void CoreUpWindScene::receiveData(QVector<QPointF> layLineData){

     this->boat->injectLaylines(layLineData);
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

LoadObstacles* CoreUpWindScene::getLoadObstacles(){
    return loadObstacles;
}

PolarDiagram* CoreUpWindScene::getPolarDiagram(){
    return pDiagram;
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

        //   longitude /= 100;
        //   latitude /= 100;
        qDebug() << Q_FUNC_INFO << latitude << ", " << longitude;
        this->boat->setGeoPosition(longitude, latitude);
    }

    //$IIHDG,15,,,7.1,W*1c
    if(text[3] == QChar('H') && text[4] == QChar('D') && (text[5] == QChar('G') || text[5] == QChar('T'))){
        QStringList strList = text.split(",");
        float heading = ((QString)strList.at(1)).toFloat();
        this->boat->setHeading(heading );

        if(this->route->getRoute().size() > 0){

            QThread* thread = new QThread;
            calculateLaylines = new CalculateLaylines();
            calculateLaylines->setPolarDiagram(this->pDiagram);
            calculateLaylines->setRoutePoints(this->route->getRoute());
            calculateLaylines->setStartPoint(*this->boat->getGeoPosition());
            calculateLaylines->moveToThread(thread);

            connect(thread, SIGNAL(started()), calculateLaylines, SLOT(start()));
            connect(calculateLaylines, SIGNAL(calculationComplete(QVector<QPointF>)), this, SLOT(receiveData(QVector<QPointF>)));
            connect(calculateLaylines, SIGNAL(finished()), thread, SLOT(quit()));
            connect(calculateLaylines, SIGNAL(finished()), calculateLaylines, SLOT(deleteLater()));
            connect(calculateLaylines, SIGNAL(finished()), thread, SLOT(deleteLater()));

            thread->start();
        }
    }
}
Q_EXPORT_PLUGIN2(coreupwindscene, CoreUpWindScene)
