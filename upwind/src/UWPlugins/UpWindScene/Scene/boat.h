#ifndef BOAT_H
#define BOAT_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include <QtGui/QtGui>

#define INITIAL_BOAT_SIZE 0.1
#define FACTOR_BOAT_SIZE 4000000
#define MIN_ZOOM_BOAT 80000
#define IMAGE_WIDTH     100
#define IMAGE_HEIGHT    200

class Boat: public QObject
{
    Q_OBJECT
public:

    void setGPSPoints();
    QVector<QPointF>getGPSPoints();

    Boat(QSize size, QRectF chartBoundaries);
    QGraphicsSvgItem *getBoatImage();
    QString getName();
    void setGeoPosition(QPointF position);
    void setGeoPosition(float longitude, float latitude);
    QPointF *getGeoPosition();

    void setView(QGraphicsView *view);
    QPointF *geoPointToPixel(QPointF *geoPoint);
    QPointF *pixelToGeoPoint(QPointF* pixelPoint);
    void updateBoatPosition();
    void setHeading(float hdg);
    float getHeading();

    void zoomIn();
    void zoomOut();

private:

    QGraphicsView *view;
    QGraphicsSvgItem *boatImage;
    QString boatName;
    QPointF *boatScenePosition, *boatGeoPosition;
    QVector<QPointF> boatPositionVector;

    float boatAngle;
    float boatScale;
    float heading;
    float zoomFactor;
    QRectF chartBoundaries;
    QSize size;
};

#endif // BOAT_H
