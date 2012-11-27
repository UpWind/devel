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

    //this->boatGeoPosition = new QPointF(25.109253, 65.013026);   // initial position Oulu
    this->boatGeoPosition = new QPointF(25.1516, 65.0146);

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

QString Boat::getName()
{
    return boatName;
}

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

/* 26112012 gps positions of boat to produce line to where we go by gps -Teemu*/
void Boat::setGPSPoints(){

    boatPositionVector.push_front(QPointF( *boatGeoPosition));
    qDebug() << " Boat::updateBoatPosition vector size is :" << boatPositionVector.size();
    qDebug() << " Boat::updateBoatPosition vector have first QpointF:" << boatPositionVector.at(0);
    if(boatPositionVector.size() >= 3){
        while(boatPositionVector.size()>=3){
            qDebug() << " Boat::updateBoatPosition vector size is :while" << boatPositionVector.size();
            qDebug() << " first" << boatPositionVector.at(0);
            qDebug() << " second" << boatPositionVector.at(1);
            boatPositionVector.pop_back();
        }
    }
    qDebug() << " Boat::updateBoatPosition vector size is : after" << boatPositionVector.size();
    qDebug() << " Boat::updateBoatPosition vector to list " << boatPositionVector.toList();


}

/* 26112012 gps positions of boat to produce line to where we go by gps -Teemu*/
QVector<QPointF> Boat::getGPSPoints(){
    return this->boatPositionVector;
}

void Boat::updateBoatPosition()
{
    this->boatScenePosition = geoPointToPixel(this->boatGeoPosition);

    setGPSPoints();

    //141112: Adjust the position so that the laylines start from the tip of the boat

    qDebug() << "BOATSCENEPOSITION->X " << boatScenePosition->x();
    qDebug() << "BOATSCENEPOSITION->Y " << boatScenePosition->y();
    float offsetx = 20;//(IMAGE_WIDTH * boatScale) / 2;
    float offsety = 0;//(IMAGE_HEIGHT * boatScale) /2;
    qDebug() << "OFFSETX " << offsetx;
    qDebug() << "OFFSETY " << offsety;

    boatImage->setPos(boatScenePosition->x() - offsetx, boatScenePosition->y() + offsety);
}

void Boat::setHeading(float hdg)
{
    this->heading = hdg;
    this->boatImage->setRotation(heading);
}

float Boat::getHeading(){
    return this->heading;
}

QPointF* Boat::geoPointToPixel(QPointF *geoPoint){ //postgrechartprovider.cpp line 503

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
