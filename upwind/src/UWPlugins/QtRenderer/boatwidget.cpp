#include "boatwidget.h"


BoatWidget::BoatWidget(QSize size, UpWindSceneInterface *uwscene, QRectF chartBoundaries)
{
    qDebug()<<Q_FUNC_INFO;
    uwScene = static_cast<CoreUpWindScene*>(uwscene); //ChartObjects not yet loaded to route.cpp
    this->chartBoundaries = chartBoundaries;
    this->size = size;

}

void BoatWidget::updateBoatPosition()
{
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


