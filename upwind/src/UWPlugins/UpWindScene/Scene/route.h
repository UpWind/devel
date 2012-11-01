#ifndef ROUTE_H
#define ROUTE_H

#include <QVector>
#include <QPointF>
#include <QLineF>
#include <QPainterPath>
#include <QThread>

#include "../ChartProviderInterface/chartobjectinterface.h"
#include "naviglineinfo.h"
#include "../../UWCore/pluginmanager.h"


class Route
{

public:

    static Route* getInstance();  //Singleton class
    Route();
    ~Route();

    void addStaticWeather(double angleWind,double speedWind);
    void clear();
    void initiate();
    void loadChartObjects( QVector<ChartObjectInterface*> cObjects);
    QVector <QPolygonF> getNavigLinesCorrected();
    QVector<QPointF> path( QPointF startPoint,QPointF endPoint,bool type);


private:

    bool addEdge(double depth, QPointF startNode, QPointF endNode,bool tolerance);
    bool addEdge(double depth,double height, QPointF startNode, QPointF endNode,bool tolerance);
    bool belongtonewsegment(QPointF point,QLineF segment, double error,QLineF *newline);
    bool belong(QPointF point, QLineF segment);
    bool createNavigLinesCorrected();
    bool intersections(double error);
    bool isEdge(QPointF startNode, QPointF endNode,bool tolerance);
    bool isPoint(QPointF a,int &position,bool tolerance );
    bool newcross(QLineF ab,QLineF cd, QPointF point);
    bool samePoint(QPointF a,QPointF b,bool tolerance );
    bool sameSegment(QLineF ab, QLineF cd);

    double distance(QPointF start, QPointF end);
    double getSlope(QLineF line);
    double distanceHaversine(QPointF start, QPointF end);

    int nearestNode( QPointF point);

    qreal min(qreal a, qreal b);
    qreal max(qreal a, qreal b);

    QPainterPath buildRectangle(QLineF line,double distance);
    QVector <QPointF> getPerpendicularPoints(double m,double b,double distance,QPointF p);
    QPointF getTackPoint(QPointF p1,QPointF p2,double twd,double angle);
    QPointF pixelToGeopoint(QPointF pixelpoint);


    void deleteEdge(QPointF startNode, QPointF endNode,bool tolerance);
    void getNavigLines();
    void getBoatInfo();
    void getBridges();
    void getTransmissionLines();
    void FilterAirWay();
    void addAirWay(double height,double width, QLineF line);
    void addAirWay(double height, QPainterPath area);
    void setBridgesFromDB();
    void setTransmissionLinesFromDB();
    void setNavigLinesCorrectedFromDB();


private:

    bool debug;

    float boatDraught;
    float boatHeight;

    static Route *instance;

    QVector<ChartObjectInterface *> chartObjects;
    //Settings* settings; //needed at all?

    QVector<QPolygonF> navlinegeom;
    QVector<double> navlinedep;
    QVector<double> navlineheight;

    QVector < QVector <NavigLineInfo> > adMatrix;
    QVector <QPointF> nodes;
    QVector <QLineF> edges;
    QVector <QPainterPath> airway;

};

#endif // ROUTE_H
