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
    //QPolygonF shortroutepoints;
    QPolygonF rightPathPoints;
    QPolygonF leftPathPoints;
    //QVector<QPolygonF> navcorrected;
    Route *route;
    ShortNavigation *routeShort;
    PolarDiagram *diagram;
    QVector<QPointF> path;
    QPointF start;
    QPointF compassStart;
    QPointF compassEnd;

    QPen longroute_pen;
    QPen shortroute_pen;
    QPen right_pen;
    QPen left_pen;
    QPen compass_pen;
    QBrush longroute_brush;
    QBrush shortroute_brush;
    QBrush right_brush;
    QBrush left_brush;
    QBrush compass_brush;
    QVector<QPointF> leftPath; //Modify so that both paths can be put into the same variable

    QVector<QPointF> pathLeft;
    QVector<QPointF> pathRight;

    bool zoomMode;
};

#endif // ROUTEWIDGET_H
