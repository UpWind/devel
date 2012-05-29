#include "googlemapchart.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDomDocument>
#include <QDomElement>
#include <QWebFrame>
#include <QWebPage>
#include <QEventLoop>
#include <math.h>

GoogleMapChart::GoogleMapChart(QWidget *parent) : QWebView(parent), pendingRequests(0)
{
    this->setWindowState(Qt::WindowFullScreen);

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    connect(this,SIGNAL(reloadMap()), this,SLOT(loadCoordinates()));
    timer = new QTimer(this);
    timer->setInterval(400);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveBoat()));
    connect(timer, SIGNAL(timeout()), this, SLOT(loadCoordinates()));
    setCoordinates(65.019442,25.402193 );
}

void GoogleMapChart::setCoordinates(qreal x, qreal y){
    target.setX(x);
    target.setY(y);

    simulator = new randomNavigate(target);
    this->page()->mainFrame()->evaluateJavaScript(
                                QString("Open(%1,%2)").arg(simulator->getCoordinates().x()).arg(simulator->getCoordinates().y()) );
}

void GoogleMapChart::geoCode(QString local){
    clearCoordinates();

    QString requestStr( tr("http://maps.google.com/maps/geo?q=%1&output=%2&key=%3")
                       .arg(local)
                       .arg("csv")
                       .arg("GOOGLE_MAPS_KEY") );

    manager->get( QNetworkRequest(requestStr) );
    ++pendingRequests;
}

void GoogleMapChart::startNavigation(){
     timer->start();
}

void GoogleMapChart::moveBoat(){
//simulator->move();
}

void GoogleMapChart::replyFinished(QNetworkReply *reply){
    QString replyStr( reply->readAll() );
    QStringList coordinateStrList = replyStr.split(",");

    if(coordinateStrList.size() == 4){
        QPointF coordinate( coordinateStrList[2].toFloat(),coordinateStrList[3].toFloat() );
        coordinates << coordinate;
    }

    --pendingRequests;
    if(pendingRequests < 1)
        emit(reloadMap());
}
void GoogleMapChart::dimensions(int a,int b){
    this->setFixedSize(a,b);
}

void GoogleMapChart::loadCoordinates()
{
  /*  foreach(QPointF point ,coordinates)
    {

    this->page()->mainFrame()->evaluateJavaScript(
                                QString("Open(%1,%2)").arg(point.x()).arg(point.y()) );
    }*/
    this->page()->mainFrame()->evaluateJavaScript(
                                QString("Open(%1,%2)").arg(simulator->getCoordinates().x()).arg(simulator->getCoordinates().y()) );
}

void GoogleMapChart::clearCoordinates()
{
    coordinates.clear();
}

