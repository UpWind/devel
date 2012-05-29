#include "boat.h"
#include "../../shared/uwmath.h"
#include "../UpWindScene/Scene/projection.h"


Boat::Boat(QSize size, QRectF chartBoundaries){

    this->boatImage = new QGraphicsSvgItem(":sailboat2.svg");
    this->boatImage->setPos(10, 10);
    this->boatScale = 0.2f;
    this->boatImage->setTransform(QTransform().scale(boatScale, boatScale));
    this->boatName = "The Flying Dutchman";

    this->boatGeoPosition = new QPointF(25.109253, 65.013026);   // initial position Oulu

    if(boatImage != NULL)
    {
        qDebug() << "Boat image loaded.";
        qDebug() << "Name: " << this->boatName;
    }

    this->chartBoundaries = chartBoundaries;
    this->size = size;
    this->zoomFactor = 1.0f;
    updateBoatPosition();

    this->setHeading(3.14);
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



void Boat::updateBoatPosition()
{
    this->boatScenePosition = geoPointToPixel(this->boatGeoPosition);

    float offsetx = (IMAGE_WIDTH * boatScale) / 2;
    float offsety = (IMAGE_HEIGHT * boatScale)/2;
    boatImage->setPos(boatScenePosition->x() + offsetx, boatScenePosition->y() + offsety);
}

void Boat::setHeading(float hdg)
{
    this->heading = hdg;
    this->boatImage->setRotation(heading);
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
