

#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

#include "routewidget.h"
#include "../shared/uwmath.h"
//#include "../../UWCore/uwcore.h"

RouteWidget::RouteWidget(QSize size, UpWindSceneInterface* uwscene, QRectF chartboundaries) :
    zoomFactor(1.0),
    rotateAngle(0.0),
    size(size),
    chartBoundaries(chartboundaries)
{
    qDebug()<<Q_FUNC_INFO;
    uwScene = static_cast<CoreUpWindScene*>(uwscene); //ChartObjects not yet loaded to route.cpp

    drawMode = false;
    simMode = false;

    longroute_pen.setColor(Qt::blue);
    longroute_pen.setWidthF(6);
    longroute_brush.setColor(Qt::blue);

    marker_pen.setColor(Qt::black);
    marker_pen.setWidthF(4);
    marker_pen.setColor(Qt::black);

}

RouteWidget::~RouteWidget(){}

QRectF RouteWidget::boundingRect() const {
    qDebug() << Q_FUNC_INFO;

    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}

QPointF* RouteWidget::geoPointToPixel(QPointF* geoPoint){

    UwMath::toConformalInverted(*geoPoint);
    geoPoint->setX((geoPoint->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width()));
    geoPoint->setY((geoPoint->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height()));

    return geoPoint;
}

QPointF* RouteWidget::pixelToGeoPoint(QPointF* pixelPoint){

    pixelPoint->setX((pixelPoint->x() / ((size.width()*zoomFactor) / chartBoundaries.width())) +  chartBoundaries.left());
    pixelPoint->setY((pixelPoint->y() / ((size.height()*zoomFactor) /  chartBoundaries.height())) + chartBoundaries.top());
    UwMath::fromConformalInverted(*pixelPoint);
    return pixelPoint;
}

void RouteWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    QPointF &longTermLastPos = this->endPoint;

    painter->setPen(longroute_pen);
    painter->setBrush(longroute_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);

    painter->drawPolyline(routepoints);
    longTermLastPos = getLongTermLastPos(&this->path);

    //Mark short term navigation starting point//First set pen and then draw a cricle
    painter->setPen(marker_pen);
    painter->setBrush(marker_brush);

    painter->drawEllipse(longTermLastPos,10.0,10.0);
}

void RouteWidget::zoomIn() {
    qDebug() << Q_FUNC_INFO;
    zoomFactor += 0.1;
    prepareGeometryChange();
}

void RouteWidget::zoomOut() {
    qDebug() << Q_FUNC_INFO;
    zoomFactor -= 0.1;
    prepareGeometryChange();
}

void RouteWidget::rotateLeft() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle -= 1;
    prepareGeometryChange();
}

void RouteWidget::rotateRight() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle += 1;
    prepareGeometryChange();
}
void RouteWidget::expand() {
    resetTransform();
    rotateAngle = 0.0;
    zoomFactor = 1.0;
    prepareGeometryChange();
}

void RouteWidget::setZoomMode(bool active) {
    zoomMode = active;
}

void RouteWidget::setSimMode(bool activate)
{
    simMode = activate;
}

void RouteWidget::drawRoute(bool activate)
{
    drawMode = activate;
}

void RouteWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) { //Listens for mouseEvents from User Interface
    this->endPoint = event->screenPos(); //End point is the new end point (last coordinate item) in path variable in short term navigation
    calculatePath();
}

QPointF RouteWidget::getLongTermLastPos(QVector<QPointF> *path){

    QPointF pathEndPoint = path->at(path->size()-2);
    qDebug()<< "Your selected display coordinates: "<<pathEndPoint;
    return pathEndPoint;
}


void RouteWidget::calculatePath(){
   QPointF start;
   QPointF &end = this->endPoint;

   //Boat position?
   QPointF *boatPosition = uwScene->getBoat()->getGeoPosition();
   qDebug() << "boatPosition: " << boatPosition->x() << ", " << boatPosition->y();

   start = *boatPosition;

   pixelToGeoPoint(&end);
   qDebug() << "Your selected geo coordinates " << end;
   Route* route = uwScene->getRoute();
   path = route->path(start, this->endPoint, 0);
   path.append(this->endPoint);

   for (int i = 0; i < path.size(); i++) {
       geoPointToPixel(&path[i]);
   }

    routepoints = QPolygonF(path);
    this->update(boundingRect());

    /*if(simMode)
    {
        //qDebug() << Q_FUNC_INFO << "Mousepressed";
        if(drawMode)
        {

            QPointF start;
            QPointF end;
            start.setY(65.004903);
            start.setX(25.383911);

            end.setY(65.198012);
            end.setX(24.246826);

            Route* route = uwScene->getRoute();
            QVector<QPointF> path=(UwMath::toConformalInverted(route->path(UwMath::fromConformalInverted( (const QPointF)start),UwMath::fromConformalInverted( (const QPointF)end),0)));

            routepoints = QPolygonF(path);
            update();

        }

    }*/
}
