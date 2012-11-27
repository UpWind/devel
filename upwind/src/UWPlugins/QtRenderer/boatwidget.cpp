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
    uwScene = static_cast<CoreUpWindScene*>(uwscene); //ChartObjects not yet loaded to route.cpp
    this->chartBoundaries = chartBoundaries;
    this->size = size;
    //271112
    gps_pen.setColor(Qt::green);
    gps_pen.setWidth(2);
    gps_brush.setColor(Qt::green);


}
BoatWidget::~BoatWidget(){

}

QRectF BoatWidget::boundingRect() const {
  //  qDebug() << Q_FUNC_INFO;

    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}


/* 26112012 gps positions of boat to produce line to where we go by gps -Teemu*/

void BoatWidget::paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    painter->setPen(gps_pen);
    painter->setBrush(gps_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(gpsPoints);

}
void BoatWidget::updateBoatGPS(){

    gpsPoints = this->getBoat()->getGPSPoints();
    qDebug() << " BoatWidget::updateBoatGPS() toList: " << gpsPoints.toList();
   // how to implement?
  //this->update(boundingRect());
}

void BoatWidget::updateBoatPosition()
{
    qDebug() << "BoatWidget::updateBoatPosition()";
    updateBoatGPS();
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


QPointF* BoatWidget::geoPointToPixel(QPointF* geoPoint){ //postgrechartprovider.cpp line 503

    // antti
    qDebug() << Q_FUNC_INFO << "geoPoint: "<< geoPoint->x() << ", " << geoPoint->y();

    UwMath::toConformalInverted(*geoPoint);
    geoPoint->setX((geoPoint->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width()));
    geoPoint->setY((geoPoint->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height()));

    // antti
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


