

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

    right_pen.setColor(Qt::yellow);
    right_pen.setWidthF(2);
    right_brush.setColor(Qt::yellow);

    left_pen.setColor(Qt::red);
    left_pen.setWidthF(2);
    left_brush.setColor(Qt::red);

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

	(void)option;
	(void)widget;


    qDebug()<<Q_FUNC_INFO;

    painter->setPen(longroute_pen);
    painter->setBrush(longroute_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(routepoints);

    //Painter specifics for right side layline
    painter->setPen(right_pen);
    painter->setBrush(right_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(/*leftPath.data(), leftPath.size()*/rightPathPoints);
    //Painter specifics for left side layline
    painter->setPen(left_pen);
    painter->setBrush(left_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(/*leftPath.data(), leftPath.size()*/leftPathPoints);

    uwScene->getBoat()->updateBoatPosition();
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

void RouteWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {

   QPointF end;

   //Boat position?
   QPointF *boatPosition = uwScene->getBoat()->getGeoPosition();
   start = *boatPosition;

   end = event->pos();

   //geoPointToPixel(&start);
   pixelToGeoPoint(&end);
  qDebug() << "Boat ui position " << end;

   Route* route = uwScene->getRoute();
   ShortNavigation* routeShort = uwScene->getShortNavigation();

   PolarDiagram *diagram = uwScene->getPolarDiagram();
   routeShort->setPolarDiagram(diagram);
   path = route->path(start, end, 0);

   this->leftPath = routeShort->startCalc(path, start);

   qDebug()<<"ShortNavig palautti: "<<leftPath;

   QPointF startPath = leftPath.at(0);
   bool startFound = false;

   pathLeft.clear();
   pathRight.clear();

   //Get left and right paths from the returned vector from shortNavig.cpp
   for (int i = 0; i < leftPath.size(); i++) {
       if((leftPath.at(i)!=startPath || i==0) && !startFound){
            pathRight.append(leftPath.at(i));

       } else if(leftPath.at(i) == startPath || startFound){
           startFound = true;
           pathLeft.append(leftPath.at(i));
       }
   }

   for (int i = 0; i < path.size(); i++) {
       geoPointToPixel(&path[i]);
   }

   routepoints = QPolygonF(path);

   for (int i = 0; i < pathRight.size(); i++) {
       qDebug() << "pathRight[i]: " << i << " " << &pathRight[i];
       geoPointToPixel(&pathRight[i]);
   }

   //Conform geoPoints to pixel in order to show in UI
   rightPathPoints = QPolygonF(pathRight);

   for (int i = 0; i < pathLeft.size(); i++) {
       qDebug() << "pathLeft[i]: " << i << " " << &pathLeft[i];
       geoPointToPixel(&pathLeft[i]);
   }
   leftPathPoints = QPolygonF(pathLeft);

   //Start painting
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
