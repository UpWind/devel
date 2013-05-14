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

    void setGPSLine();
    void setOffSet();
    QGraphicsLineItem getGPSLine();

    Boat(QSize size, QRectF chartBoundaries);
    QGraphicsSvgItem *getBoatImage();
    QGraphicsLineItem *getBoatCompass();
    QGraphicsLineItem *getBoatGPS();
    QGraphicsPolygonItem *getPortLayline();
    QGraphicsPolygonItem *getStarBoardLayline();
    QString getName();
    void setGeoPosition(QPointF position);
    void setGeoPosition(float longitude, float latitude);
    QPointF getGeoPosition();

    void setView(QGraphicsView *view);
    QPointF geoPointToPixel(const QPointF &geoPoint);
    QPointF* pixelToGeoPoint(QPointF* pixelPoint);
    void updateBoatPosition();
    void setHeading(float hdg);
    float getHeading();
    void injectLaylines(QVector<QPointF> laylines);

    void zoomIn();
    void zoomOut();

    void setZoomFactor(qreal zoomFactor);

    QPointF getBoatScenePosition() const;

signals:
    void boatPositionChanged();

private:

     QTransform transformers;
    QGraphicsView *view;
    QGraphicsSvgItem *boatImage;
    QString boatName;
    QPointF boatScenePosition;
    QPointF boatGeoPosition;
    QPointF firstScenePosition;
    QPointF secondScenePosition;
    QPointF firstPoint;
    QPointF secondPoint;
    QPointF endCompassPoint;
    QPointF endSceneCompassPoint;
    QGraphicsLineItem *compass;
    QGraphicsLineItem *gps;
    QGraphicsPolygonItem *portLaylineItem;
    QGraphicsPolygonItem *starBoardLaylineItem;
    QPen compass_pen;
    QPen gps_pen;
    QPen starBoardLayline_pen;
    QPen portLayline_pen;
    QVector<QPointF> boatPositionVector;

    QBrush starBoard_brush;
    QBrush port_brush;
    QBrush compass_brush;

    float boatAngle;
    float boatScale;
    float heading;
    float zoomFactor;
    QRectF chartBoundaries;
    QSize size;

    QVector<QPointF> layLinePoints;
    QVector<QPointF> tempPath; //Modify so that both paths can be put into the same variable
    QVector<QPointF> pathStarBoard;
    QVector<QPointF> pathPort;
    QPolygonF starBoardLayline;
    QPolygonF portLayline;
    QVector<QPolygonF> navcorrected;

    void setLaylines();
    void geoLaylineToPixel(QPointF* pixelPoint);

};

#endif // BOAT_H

