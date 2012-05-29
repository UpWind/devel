#ifndef BOATWIDGET_H
#define BOATWIDGET_H

#include "../shared/uwmath.h"
#include "../../UWCore/uwcore.h"


class BoatWidget
{
public:
    explicit BoatWidget(QSize size, UpWindSceneInterface* uwscene, QRectF chartBoundaries);

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


};

#endif // BOATWIDGET_H
