#include "boat.h"
#include "../../shared/uwmath.h"
#include "../UpWindScene/Scene/projection.h"


Boat::Boat(QSize size, QRectF chartBoundaries){

    this->boatImage = new QGraphicsSvgItem(":sailboat2.svg");
    this->boatImage->setPos(0, 0);
    this->boatScale = 0.2f;
    this->boatImage->setTransform(QTransform().scale(boatScale, boatScale));
    this->boatName = "The Flying Dutchman";
    this->boatImage->setOpacity(0.7);
    this->gps = new QGraphicsLineItem();
    this->compass = new QGraphicsLineItem();

   // compass->setLine(boatImage->x(),boatImage->y(),100,100);
    compass_pen.setColor(Qt::green);
    compass_pen.setWidth(4);
    compass->setPen(compass_pen);

    //291112 gps-line pen and brush settings
    gps_pen.setColor(Qt::magenta);
    gps_pen.setWidth(4);
    gps->setPen(gps_pen);
    gps->setOpacity(0.7);

    // Initial position Oulu:
    this->boatGeoPosition = new QPointF(25.109253, 65.013026);

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
    this->setHeading(290);
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

    if(*boatGeoPosition != boatPositionVector.at(0)){

        boatPositionVector.push_front(QPointF( *boatGeoPosition));
        if(boatPositionVector.size() >= 3){
            while(boatPositionVector.size()>=3){

                qDebug() << " Boat::updateBoatPosition vector size is :while" << boatPositionVector.size();
                qDebug() << " first" << boatPositionVector.at(0);
                qDebug() << " second" << boatPositionVector.at(1);
                boatPositionVector.pop_back();
            }
        }
    }
    qDebug() << " Boat::updateBoatPosition vector size is : after" << boatPositionVector.size();
    qDebug() << " Boat::updateBoatPosition vector to list " << boatPositionVector.toList();

    if(boatPositionVector.size() >= 2 ){
        firstPoint = boatPositionVector.at(0);
        secondPoint = boatPositionVector.at(1);
       this->firstScenePosition = *geoPointToPixel(&firstPoint);
       this->secondScenePosition = *geoPointToPixel(&secondPoint);
        qDebug()<< "JEE JEE 1" << firstScenePosition;
        qDebug()<< "JEE JEE 2" << secondScenePosition;

        QLineF gpsLineGeoPointToPixel(firstScenePosition,secondScenePosition);
        gpsLineGeoPointToPixel.setLength(50);
        qDebug()<<"gpsLineGeoPointToPixel x1:"<<gpsLineGeoPointToPixel.x1();
        qDebug()<<"gpsLineGeoPointToPixel y1:"<<gpsLineGeoPointToPixel.y1();
        qDebug()<<"gpsLineGeoPointToPixel x2:"<<gpsLineGeoPointToPixel.x2();
        qDebug()<<"gpsLineGeoPointToPixel y2:"<<gpsLineGeoPointToPixel.y2();
        gps->setLine(gpsLineGeoPointToPixel);
    }else{
        //atm values are just for testing
        gps->setLine(0,0,50,50);
    }
}


void Boat::updateBoatPosition()
{
    this->boatScenePosition = geoPointToPixel(this->boatGeoPosition);

    setGPSLine();

    //141112: Adjust the position so that the laylines start from the tip of the boat:

    qDebug() << "BOATSCENEPOSITION->X " << boatScenePosition->x();
    qDebug() << "BOATSCENEPOSITION->Y " << boatScenePosition->y();
    float offsetx = 20;//(IMAGE_WIDTH * boatScale) / 2;
    float offsety = 0;//(IMAGE_HEIGHT * boatScale) /2;
    qDebug() << "OFFSETX " << offsetx;
    qDebug() << "OFFSETY " << offsety;

    boatImage->setPos(boatScenePosition->x() - offsetx, boatScenePosition->y() + offsety);
    //compass->setPos(boatScenePosition->x() - offsetx, boatScenePosition->y() + offsety);

    QPointF startPoint(boatScenePosition->x(), boatScenePosition->y());

    float angle = this->heading;
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

    compass->setLine(startPoint.x(), startPoint.y(), endx, endy );

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
