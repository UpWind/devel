#include "boat.h"
#include "../../shared/uwmath.h"
#include "../UpWindScene/Scene/projection.h"

Boat::Boat(QSize size, QRectF chartBoundaries){

    this->boatImage = new QGraphicsSvgItem(":sailboat2.svg");
    this->boatImage->setPos(0, 0);
    this->boatScale = 0.2f;

    //Take boat image and set origin point to its middle point
    boatImage->setTransformOriginPoint(50,100);
    transformers.scale(boatScale, boatScale);

    QRectF boatRect = boatImage->mapRectToScene(boatImage->boundingRect());
    this->boatImage->setTransform(transformers);
    boatRect = boatImage->mapRectToScene(boatImage->boundingRect());

    this->boatName = "The Flying Dutchman";
    this->boatImage->setOpacity(0.7);
//    this->gps = new QGraphicsLineItem();
    this->compass = new QGraphicsLineItem();
    this->starBoardLaylineItem = new QGraphicsPolygonItem();
    this->portLaylineItem = new QGraphicsPolygonItem();

    starBoardLayline_pen.setColor(Qt::yellow);
    starBoardLayline_pen.setWidth(2);
    starBoard_brush.setColor(Qt::yellow);
    portLayline_pen.setColor(Qt::red);
    portLayline_pen.setWidth(2);
    port_brush.setColor(Qt::red);
    starBoardLaylineItem->setPen(starBoardLayline_pen);
    portLaylineItem->setPen(portLayline_pen);


    // compass->setLine(boatImage->x(),boatImage->y(),100,100);
    compass_pen.setColor(Qt::green);
    compass_pen.setWidth(4);
    compass->setPen(compass_pen);

    //291112 gps-line pen and brush settings
//    gps_pen.setColor(Qt::magenta);
//    gps_pen.setWidth(4);
//    gps->setPen(gps_pen);
//    gps->setOpacity(0.7);

    // Initial position Oulu
    this->boatGeoPosition = new QPointF(25.109253, 65.013026 /*25.2715, 65.1126*/);

    if(boatImage != NULL)
    {
        qDebug() << "Boat image loaded.";
        qDebug() << "Name: " << this->boatName;
    }

    this->chartBoundaries = chartBoundaries;
    this->size = size;
    this->zoomFactor = 1.0f;
    updateBoatPosition();

    //141112: Rotate boat, should rotate automatically depending on the laylines??
    //071212: values for setting boat image right place
    this->setHeading(222);

}

QGraphicsSvgItem *Boat::getBoatImage()
{
    return boatImage;
}

QGraphicsLineItem *Boat::getBoatCompass()
{
    return compass;
}
//QGraphicsLineItem *Boat::getBoatGPS()
//{
//    return gps;
//}

QGraphicsPolygonItem *Boat::getPortLayline(){

    return portLaylineItem;
}

QGraphicsPolygonItem *Boat::getStarBoardLayline(){


    return starBoardLaylineItem;
}

QString Boat::getName()
{
    return boatName;
}

//The first setGeoPosition() updates them according to updated data.
//the second one sets the hard-coded values for the position of the boat:

void Boat::setGeoPosition(float longitude, float latitude)
{
    this->boatGeoPosition = new QPointF(longitude, latitude);
    updateBoatPosition();
}

void Boat::setGeoPosition(QPointF position)
{
    this->boatGeoPosition = &position;
    updateBoatPosition();
}

//void Boat::setGPSLine(){

//    //Wait until there is at least two points in the vector
//    //then prepend and append the vector with incoming values

//    if (boatPositionVector.empty() || boatPositionVector.at(0) != *boatGeoPosition) {
//        boatPositionVector.push_front(QPointF(*boatGeoPosition));
//        while (boatPositionVector.size() > 2) {
//            boatPositionVector.pop_back();
//        }
//    }

//    if (boatPositionVector.size() == 2) {
//        firstPoint = boatPositionVector.at(0);
//        secondPoint = boatPositionVector.at(1);
//        this->firstScenePosition = *geoPointToPixel(&firstPoint);
//        this->secondScenePosition = *geoPointToPixel(&secondPoint);

//        float angle =  UwMath::getPolarAngleWithDestiny(this->firstPoint, this->secondPoint) - 180;
//        QRectF boatRect = boatImage->mapRectToScene(boatImage->boundingRect());
//        QPointF startPoint(boatRect.center().x(), boatRect.center().y());
//        QPointF endPoint = this->calculateEndPoint(angle, 50, startPoint);

//        QLineF gpsLineGeoPointToPixel(startPoint,endPoint);
//        gpsLineGeoPointToPixel.setLength(50);

//        gps->setLine(gpsLineGeoPointToPixel);
//    } else {
//        //atm line goes from boat to middle of screen, before boat have moved and it get new boatGeoPositions
//        firstPoint = *boatGeoPosition;
//        this->firstScenePosition = *geoPointToPixel(&firstPoint);
//        QPointF screenMiddlePoint;
//        float middlepointX = ((QApplication::desktop()->screenGeometry().width())/2);
//        float middlepointY = ((QApplication::desktop()->screenGeometry().height())/2);
//        screenMiddlePoint.setX(middlepointX);
//        screenMiddlePoint.setY(middlepointY);
//        gps->setLine(firstScenePosition.x(),firstScenePosition.y(), screenMiddlePoint.x(), screenMiddlePoint.y());
//        //    if needed line to go from boat to left upper corner, before boat have moved and it get new boatGeoPositions
//        //    gps->setLine(firstScenePosition.x(),firstScenePosition.y(),0,0);
//    }
//}

void Boat::setOffSet(){

    boatImage->setPos(boatScenePosition->x() - 10, boatScenePosition->y() - 20);
}

void Boat::updateBoatPosition()
{
    this->boatScenePosition = geoPointToPixel(this->boatGeoPosition);

    //Katja 12.12.12:

//    setGPSLine();
    setOffSet();

    QRectF boatRect = boatImage->mapRectToScene(boatImage->boundingRect());
    QPointF startPoint(boatRect.center().x(), boatRect.center().y());

    float angle = this->heading;
    int lineLength=50;
    QPointF endPoint = this->calculateEndPoint(angle, lineLength, startPoint);

    compass->setLine(boatRect.center().x(), boatRect.center().y(), endPoint.x(), endPoint.y());
    setLaylines();
}

QPointF Boat::calculateEndPoint(float angle, float lineLength, QPointF startPoint){


    // math to start from found at:
    // http://mathhelpforum.com/geometry/86432-endpoint-based-length-angle.html
    //Adjusted to conform with radians
    QPointF endPoint;
    if (angle > 0 && angle < 90){
        endPoint.setX(startPoint.x() + (lineLength * sin((angle)/180*M_PI)));
        endPoint.setY(startPoint.y() - (lineLength * cos((angle)/180*M_PI)));
    } else if (angle > 90 && angle < 180){
        endPoint.setX(startPoint.x() - (lineLength * sin((angle-180)/180*M_PI)));
        endPoint.setY(startPoint.y() + (lineLength * cos((angle-180)/180*M_PI)));
    } else if (angle > 180 && angle < 270){
        endPoint.setX(startPoint.x() - (lineLength * sin((angle-180)/180*M_PI)));
        endPoint.setY(startPoint.y() + (lineLength * cos((angle-180)/180*M_PI)));
    } else if (angle > 270 && angle < 360){
        endPoint.setX(startPoint.x() + (lineLength * sin((angle)/180*M_PI)));
        endPoint.setY(startPoint.y() - (lineLength * cos((angle)/180*M_PI)));
    } else if (angle == 0 || angle == 360) {
        endPoint.setX(startPoint.x());
        endPoint.setY(startPoint.y() - lineLength);
    } else if (angle == 90){
        endPoint.setX(startPoint.x() + lineLength);
        endPoint.setY(startPoint.y());
    } else if (angle == 180){
        endPoint.setX(startPoint.x());
        endPoint.setY(startPoint.y() + lineLength);
    } else if (angle == 270){
        endPoint.setX(startPoint.x() - lineLength);
        endPoint.setY(startPoint.y());
    }

    return endPoint;

    emit boatPositionChanged();
}

void Boat::setHeading(float hdg)
{
    this->heading = hdg;
    this->boatImage->setRotation(heading);
}

void Boat::injectLaylines(QVector<QPointF> laylines){

    this->layLinePoints = laylines;

    QPointF startPath = layLinePoints.at(0);
    bool startFound = false;

    pathPort.clear();
    pathStarBoard.clear();


    for (int i = 0; i < layLinePoints.size(); i++) {
        if((layLinePoints.at(i)!=startPath || i==0) && !startFound){
            pathStarBoard.append(layLinePoints.at(i));

        } else if(layLinePoints.at(i) == startPath || startFound){
            startFound = true;
            pathPort.append(layLinePoints.at(i));
        }
    }


    //    091112: leftPath data to pixeldata for drawing
    for (int i = 0; i < pathPort.size(); i++) {
        geoLaylineToPixel(&pathPort[i]);
    }
    portLayline = QPolygonF(pathPort);

    for (int i = 0; i < pathStarBoard.size(); i++) {
        geoLaylineToPixel(&pathStarBoard[i]);
    }
    starBoardLayline = QPolygonF(pathStarBoard);


   // this->writeToFile("Drawing to ui: " + QString::number(this->timer->nsecsElapsed()/1E6) + '\t');

}

void Boat::geoLaylineToPixel(QPointF *geoPoint){

    UwMath::toConformalInverted(*geoPoint);
    geoPoint->setX((geoPoint->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width()*zoomFactor));
    geoPoint->setY((geoPoint->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height()*zoomFactor));

}

void Boat::setLaylines(){

    portLaylineItem->setPolygon(this->portLayline);
    starBoardLaylineItem->setPolygon(this->starBoardLayline);

}

float Boat::getHeading(){
    return this->heading;
}
//The following line is a copied from postgrechartprovider.cpp:
    QPointF* Boat::geoPointToPixel(QPointF *geoPoint){

        double x=geoPoint->x();
        double y=geoPoint->y();

    QPointF *scenePos1 = new QPointF(x, y);

    UwMath::toConformalInverted(scenePos1);

    scenePos1->setX((scenePos1->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width() * zoomFactor));
    scenePos1->setY((scenePos1->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height() * zoomFactor));

    return scenePos1;
}


QPointF* Boat::pixelToGeoPoint(QPointF* pixelPoint){

    pixelPoint->setX((pixelPoint->x() / (size.width() / chartBoundaries.width())) +  chartBoundaries.left());
    pixelPoint->setY((pixelPoint->y() / (size.height() /  chartBoundaries.height())) + chartBoundaries.top());
    UwMath::fromConformalInverted(*pixelPoint);
    return pixelPoint;
}


void Boat::setView(QGraphicsView *view)
{
    //    qDebug() << "Set view!";
    this->view = view;
}

QPointF *Boat::getGeoPosition()
{
    return boatGeoPosition;
}

void Boat::zoomIn()
{
    zoomFactor +=0.1f;
    updateBoatPosition();
    injectLaylines(this->layLinePoints);
}

void Boat::zoomOut()
{
    zoomFactor -=0.1f;
    updateBoatPosition();
    injectLaylines(this->layLinePoints);
}

void Boat::setZoomFactor(qreal zoomFactor)
{
    this->zoomFactor = zoomFactor;
    updateBoatPosition();
    injectLaylines(this->layLinePoints);
}

QPointF Boat::getBoatScenePosition() const
{
    return *boatScenePosition;
}

