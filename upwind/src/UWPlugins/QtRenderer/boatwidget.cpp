#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "boatwidget.h"
#include "../shared/uwmath.h"




BoatWidget::BoatWidget(QSize size, UpWindSceneInterface *uwscene, QRectF chartBoundaries)
{
    qDebug()<<Q_FUNC_INFO;

    //ChartObjects not yet loaded to route.cpp:
    uwScene = static_cast<CoreUpWindScene*>(uwscene);
    this->chartBoundaries = chartBoundaries;
    this->size = size;
//    //271112
//    gps_pen.setColor(Qt::green);
//    gps_pen.setWidth(2);
//    gps_brush.setColor(Qt::green);


}
BoatWidget::~BoatWidget(){

}

QRectF BoatWidget::boundingRect() const {
  //  qDebug() << Q_FUNC_INFO;

    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}


/* 26112012 GPS positions of boat to produce line to where we go by GPS -Teemu*/

void BoatWidget::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget) {
/*    (void)option;
    (void)widget;

    painter->setPen(gps_pen);
    painter->setBrush(gps_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(gpsPoints);

    compass_pen.setColor(Qt::green);
    compass_pen.setWidth(2);
    compass_brush.setColor(Qt::green);
    painter->setPen(compass_pen);
    painter->setBrush(compass_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->rotate(rotateAngle);

    //painter->drawLine(this->getBoat()->getGeoPosition()->x(), this->getBoat()->getGeoPosition()->y(), 0, 0 );

    qDebug() << "#################################################";
    painter->drawLine( 0, 0,500,500 );*/

}
//void BoatWidget::updateBoatGPS(){

////    gpsPoints = this->getBoat()->getGPSPoints();
//    qDebug() << " BoatWidget::updateBoatGPS() toList: " << gpsPoints.toList();
//    // We are still wondering how to implement the following:
//    //this->update(boundingRect());
//}

void BoatWidget::updateBoatPosition()
{
    qDebug() << "BoatWidget::updateBoatPosition()";
//    updateBoatGPS();
//    QPointF *p = new QPointF(25.109253, 65.013026);
//    QPointF *scenePos = geoPointToPixel(boat->getGeoPosition());
//    qDebug() << Q_FUNC_INFO << "scenePos: " << scenePos->x() << ", " << scenePos->y();
//    boat->getBoatImage()->setPos(scenePos->x(), scenePos->y());
}

void BoatWidget::setBoat(Boat *boat)
{
    this->boat = boat;
}

Boat *BoatWidget::getBoat()
{
    return this->boat;
}

//Line copied from postgrechartprovider.cpp:
QPointF* BoatWidget::geoPointToPixel(QPointF* geoPoint){

    qDebug() << Q_FUNC_INFO << "geoPoint: "<< geoPoint->x() << ", " << geoPoint->y();

    UwMath::toConformalInverted(*geoPoint);
    geoPoint->setX((geoPoint->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width()));
    geoPoint->setY((geoPoint->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height()));

    qDebug() << Q_FUNC_INFO << "scenePoint: "<< geoPoint->x() << ", " << geoPoint->y();

    return geoPoint;
}

QPointF* BoatWidget::pixelToGeoPoint(QPointF* pixelPoint){

    pixelPoint->setX((pixelPoint->x() / (size.width() / chartBoundaries.width())) +  chartBoundaries.left());
    pixelPoint->setY((pixelPoint->y() / (size.height() /  chartBoundaries.height())) + chartBoundaries.top());
    UwMath::fromConformalInverted(*pixelPoint);
    return pixelPoint;
}

void BoatWidget::zoomIn()
{
    boat->zoomIn();
}

void BoatWidget::zoomOut()
{
    boat->zoomOut();
}


