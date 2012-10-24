#ifndef ROUTEWIDGET_H
#define ROUTEWIDGET_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QGraphicsItem>
#include "../UpWindScene/Scene/route.h" //apra: testing

class RouteWidget : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit RouteWidget(QSize size, UpWindSceneInterface* uwscene, QRectF chartBoundaries);
    ~RouteWidget();
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    QRectF boundingRect() const;
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
    void expand();
    void setZoomMode(bool);
    void setSimMode(bool);
    void drawRoute(bool);
    QPointF *geoPointToPixel(QPointF* geoPoint);
    QPointF *pixelToGeoPoint(QPointF* pixelPoint);
    QPointF getLongTermLastPos(QVector<QPointF> *path);
    QPointF getClosestStartPoint();

signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void calculatePath();
    void geopointToPixel(QPointF *geopoint);
    
public slots:
    
private:
    CoreUpWindScene* uwScene;
    QRectF chartBoundaries;
    qreal zoomFactor;
    qreal rotateAngle;
    QSize size;
    bool simMode;
    bool drawMode;
    QPolygonF routepoints;
    //QVector<QPolygonF> navcorrected;
    Route *route;
    QVector<QPointF> path;
    QPointF endPoint; //end point for short term navigation
    QPointF pathEndPoint; //end point for long term navigation
     QPointF mousePosition;
    QPen longroute_pen;
    QBrush longroute_brush;

    QPen marker_pen;
    QBrush marker_brush;

    bool zoomMode;
};

#endif // ROUTEWIDGET_H
