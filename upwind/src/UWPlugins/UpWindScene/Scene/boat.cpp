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
    this->gps = new QGraphicsLineItem();
    this->compass = new QGraphicsLineItem();

    compass_pen.setColor(Qt::green);
    compass_pen.setWidth(4);
    compass->setPen(compass_pen);

    //291112 gps-line pen and brush settings
    gps_pen.setColor(Qt::magenta);
    gps_pen.setWidth(4);
    gps->setPen(gps_pen);
    gps->setOpacity(0.7);

    // Initial position Oulu:
    this->boatGeoPosition = new QPointF(/*25.109253, 65.013026*/ 25.2715, 65.1126);

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
QGraphicsLineItem *Boat::getBoatGPS()
{
    return gps;
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

void Boat::setGPSLine(){

    //Wait until there is at least two points in the vector
    //then prepend and append the vector with incoming values

    if(*boatGeoPosition != boatPositionVector.at(0)){

        boatPositionVector.push_front(QPointF( *boatGeoPosition));
        if(boatPositionVector.size() >= 3){
            while(boatPositionVector.size()>=3){
                boatPositionVector.pop_back();
            }
        }
    }

    if(boatPositionVector.size() >= 2 ){
        firstPoint = boatPositionVector.at(0);
        secondPoint = boatPositionVector.at(1);
        this->firstScenePosition = *geoPointToPixel(&firstPoint);
        this->secondScenePosition = *geoPointToPixel(&secondPoint);

        QLineF gpsLineGeoPointToPixel(firstScenePosition,secondScenePosition);
        gpsLineGeoPointToPixel.setLength(50);
        gps->setLine(gpsLineGeoPointToPixel);
    }else{
        //atm line goes from boat to middle of screen, before boat have moved and it get new boatGeoPositions
        firstPoint = *boatGeoPosition;
        this->firstScenePosition = *geoPointToPixel(&firstPoint);
        float middlepointX = ((QApplication::desktop()->screenGeometry().width())/2);
        float middlepointY = ((QApplication::desktop()->screenGeometry().height())/2);
        screenMiddlePoint.setX(middlepointX);
        screenMiddlePoint.setY(middlepointY);
        qDebug() << "middlePointX" << screenMiddlePoint;
        gps->setLine(firstScenePosition.x(),firstScenePosition.y(), screenMiddlePoint.x(), screenMiddlePoint.y());
        //    if needed line to go from boat to left upper corner, before boat have moved and it get new boatGeoPositions
        //    gps->setLine(firstScenePosition.x(),firstScenePosition.y(),0,0);
    }
}

void Boat::setOffSet(){

     boatImage->setPos(boatScenePosition->x() - 10, boatScenePosition->y() - 20);
}

void Boat::updateBoatPosition()
{
    this->boatScenePosition = geoPointToPixel(this->boatGeoPosition);

    //Katja 12.12.12:

    setGPSLine();
    setOffSet();

    QRectF boatRect = boatImage->mapRectToScene(boatImage->boundingRect());
    QPointF startPoint(boatRect.center().x(), boatRect.center().y());

    float angle = this->heading;
    float endx = 0;
    float endy = 0;
    int lineLength=50;

    // math to start from found at:
    // http://mathhelpforum.com/geometry/86432-endpoint-based-length-angle.html
    //Adjusted to conform with radians
    if (angle > 0 && angle < 90){
        endx = startPoint.x() + (lineLength * sin((angle)/180*M_PI));
        endy = startPoint.y() - (lineLength * cos((angle)/180*M_PI));
    } else if (angle > 90 && angle < 180){
        endx = startPoint.x() - (lineLength * sin((angle-180)/180*M_PI));
        endy = startPoint.y() + (lineLength * cos((angle-180)/180*M_PI));
    } else if (angle > 180 && angle < 270){
        endx = startPoint.x() - (lineLength * sin((angle-180)/180*M_PI));
        endy = startPoint.y() + (lineLength * cos((angle-180)/180*M_PI));
    } else if (angle > 270 && angle < 360){
        endx = startPoint.x() + (lineLength * sin((angle)/180*M_PI));
        endy = startPoint.y() - (lineLength * cos((angle)/180*M_PI)) ;
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

    compass->setLine(boatRect.center().x(), boatRect.center().y(), endx, endy );

}

void Boat::setHeading(float hdg)
{
    this->heading = hdg;
    this->boatImage->setRotation(heading);
}

float Boat::getHeading(){
    return this->heading;
}
//The following line is a copied from postgrechartprovider.cpp:
QPointF* Boat::geoPointToPixel(QPointF *geoPoint){

    QPointF *scenePos = new QPointF(geoPoint->x(), geoPoint->y());
    //    QPointF *scenePos = geoPoint;

    UwMath::toConformalInverted(*scenePos);
    scenePos->setX((scenePos->x() - chartBoundaries.left()) * (size.width() / chartBoundaries.width() * zoomFactor));
    scenePos->setY((scenePos->y() - chartBoundaries.top()) * (size.height()/  chartBoundaries.height() * zoomFactor));

    return scenePos;
}

QPointF* Boat::pixelToGeoPoint(QPointF* pixelPoint){

    pixelPoint->setX((pixelPoint->x() / (size.width() / chartBoundaries.width())) +  chartBoundaries.left());
    pixelPoint->setY((pixelPoint->y() / (size.height() /  chartBoundaries.height())) + chartBoundaries.top());
    UwMath::fromConformalInverted(*pixelPoint);
    return pixelPoint;
}


void Boat::setView(QGraphicsView *view)
{
    qDebug() << "Set view!";
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
}

void Boat::zoomOut()
{
    zoomFactor -=0.1f;
    updateBoatPosition();
}

