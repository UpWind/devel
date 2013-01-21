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

signals:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
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

    Route *route;
    QVector<QPointF> path;
    QPointF start;

    QPen longroute_pen;

    QBrush longroute_brush;


    QVector<QPointF> pathLeft;
    QVector<QPointF> pathRight;

    bool zoomMode;
};

#endif // ROUTEWIDGET_H
