

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

    compass_pen.setColor(Qt::magenta);
    compass_pen.setWidth(2);
    compass_brush.setColor(Qt::magenta);

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

    painter->setPen(right_pen);
    painter->setBrush(right_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(/*leftPath.data(), leftPath.size()*/rightPathPoints);

    painter->setPen(left_pen);
    painter->setBrush(left_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    //painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    painter->drawPolyline(/*leftPath.data(), leftPath.size()*/leftPathPoints);

    // 271112 Compass line
    QPointF *boatPosition = uwScene->getBoat()->getGeoPosition();
    QPointF startPoint = *boatPosition;
    geoPointToPixel(&startPoint);

    float angle = uwScene->getBoat()->getHeading();
    qDebug() << "angle: " << angle;
    float endx = 0;
    float endy = 0;
    int lineLength=50;

    // math to start from found at:
    // http://mathhelpforum.com/geometry/86432-endpoint-based-length-angle.html
    if (angle > 0 && angle < 90){
        endx = startPoint.x() + (lineLength * cos(0-angle));
        endy = startPoint.y() + (lineLength * sin(0-angle));
    } else if (angle > 90 && angle < 180){
        endx = startPoint.x() - (lineLength * cos(angle-180));
        endy = startPoint.y() + (lineLength * sin(angle-180));
    } else if (angle > 180 && angle < 270){
        endx = startPoint.x() - (lineLength * sin(angle));
        endy = startPoint.y() - (lineLength * cos(angle));
    } else if (angle > 270 && angle < 360){
        endx = startPoint.x() + (lineLength * sin(0-angle));
        endy = startPoint.y() - (lineLength * cos(0-angle)) ;
    } else if (angle == 0 || angle == 360) {
        endx = startPoint.x();
        endy = startPoint.y() - lineLength;
    } else if (angle == 90){
        endx = startPoint.x() + lineLength;
        endy = startPoint.y();
    } else if (angle == 180){
        endx = startPoint.x();
        endy = startPoint.y() + lineLength;
    } else if (angle == 270){
        endx = startPoint.x() - lineLength;
        endy = startPoint.y();
    }

    painter->setPen(compass_pen);
    painter->setBrush(compass_brush);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->rotate(rotateAngle);
    painter->drawLine(startPoint.x(), startPoint.y(), endx, endy );

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
   qDebug() << "***********************boatPosition: " << boatPosition->x() << ", " << boatPosition->y();
   start = *boatPosition;

   end = event->pos();
   //geoPointToPixel(&start);
   pixelToGeoPoint(&end);


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

   for (int i = 0; i < leftPath.size(); i++) {
       if((leftPath.at(i)!=startPath || i==0) && !startFound){
            pathRight.append(leftPath.at(i));

       } else if(leftPath.at(i) == startPath || startFound){
           startFound = true;
           pathLeft.append(leftPath.at(i));
       }
   }

qDebug() <<"pathRight: "<<pathRight;
qDebug() << "pathLeft: " << pathLeft;

   for (int i = 0; i < path.size(); i++) {
       geoPointToPixel(&path[i]);
   }

   routepoints = QPolygonF(path);

   //091112: leftPath data to pixeldata for drawing
//   for (int i = 0; i < leftPath.size(); i++) {
//       qDebug() << "leftPath[i]: " << i << " " << &leftPath[i];
//       geoPointToPixel(&leftPath[i]);
//   }
//   shortroutepoints = QPolygonF(leftPath);

   for (int i = 0; i < pathRight.size(); i++) {
       qDebug() << "pathRight[i]: " << i << " " << &pathRight[i];
       geoPointToPixel(&pathRight[i]);
   }
   rightPathPoints = QPolygonF(pathRight);

   for (int i = 0; i < pathLeft.size(); i++) {
       qDebug() << "pathLeft[i]: " << i << " " << &pathLeft[i];
       geoPointToPixel(&pathLeft[i]);
   }
   leftPathPoints = QPolygonF(pathLeft);

   //compassStart = leftPathPoints.at(0);
   //compassEnd = leftPathPoints.at(leftPathPoints.size()-1);

   //09112: print out both route pixels for testing purposes
   qDebug() << "ROUTEPOINTS: " << routepoints;
//   qDebug() << "SHORTROUTEPOINTS: " << shortroutepoints;


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
