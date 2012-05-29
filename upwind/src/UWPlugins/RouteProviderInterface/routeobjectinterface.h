#ifndef ROUTEOBJECTINTERFACE_H
#define ROUTEOBJECTINTERFACE_H

#include <QVector>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

class RouteObjectInterface{
public:
    enum RouteObjectType {
        Point,
        Line,
        Polygon
    } typedef RouteObjectType;

    explicit RouteObjectInterface(QVector<QPolygonF> geom, QString tabName){}
    virtual RouteObjectType getType() const = 0;
    virtual QVector<QPolygonF> getGeometry() const = 0;
    virtual QPen getPen() const = 0;
    virtual QBrush getBrush() const = 0;
    virtual QString getTableName() const = 0;
};

#endif // ROUTEOBJECTINTERFACE_H
