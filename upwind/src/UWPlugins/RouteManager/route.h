#ifndef ROUTE_H
#define ROUTE_H

#include <QObject>
#include <QPointF>
class Route : public QObject
{
    Q_OBJECT
public:
    explicit Route(QObject *parent = 0);
    QList<QPointF*> getRoutePoints();
    void setRoutePoints(QList<QPointF*> list);

private:
    QList<QPointF*> routePoints;

signals:

public slots:



};

#endif // ROUTE_H
