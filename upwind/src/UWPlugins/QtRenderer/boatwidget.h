#ifndef BOATWIDGET_H
#define BOATWIDGET_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QGraphicsItem>

#include "../shared/uwmath.h"
#include "../../UWCore/uwcore.h"


class BoatWidget
{
public:
    explicit BoatWidget(QSize size, UpWindSceneInterface* uwscene, QRectF chartBoundaries);

    //271112 for gps here
    ~BoatWidget();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    void getLaylines(QPolygonF route);
    QRectF boundingRect() const;
    void updateBoatGPS();
    //271112 to here

    void updateBoatPosition();
    void setBoat(Boat *boat);
    Boat *getBoat();

    QPointF *geoPointToPixel(QPointF* geoPoint);
    QPointF *pixelToGeoPoint(QPointF* pixelPoint);

    void zoomIn();
    void zoomOut();

private:
    CoreUpWindScene* uwScene;
    QRectF chartBoundaries;
    QSize size;
    Boat *boat;

    //271112
    qreal zoomFactor;
    qreal rotateAngle;
    QPen gps_pen;
    QBrush gps_brush;
    QVector<QPointF> gpsPoints;
    //271112

};

#endif // BOATWIDGET_H
