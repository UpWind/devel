#ifndef CALCULATELAYLINES_H
#define CALCULATELAYLINES_H

#include <QPolygonF>
#include <QPointF>
#include <QString>
#include <QLineF>
#include <QVector>
#include <QObject>
#include <stdlib.h>
#include <stdio.h>
#include "../ChartProviderInterface/chartobjectinterface.h"

#include "libpq-fe.h"
#include "../shared/uwmath.h"
#include "polardiagram.h"

class CalculateLaylines : public QObject
{
    Q_OBJECT
public:

    CalculateLaylines(QObject* parent = 0);
    ~CalculateLaylines();

    static QString buildWKTPolygon( const QPolygonF &rhomboid );
    static QString buildWKTPolygon( const QPolygonF &polygon, const QPointF &centroid,
                                    const double &perimeter, const float &offset );
    static QString buildWKTPolygon( const QPointF &point, const float &offset );
    static QString buildWKTLine( const QPointF &p1, const QPointF &p2 );
    static QString buildWKTPoint( const QPointF &p1 );
    void setPolarDiagram(PolarDiagram *diagram);

    bool publicCheckIntersection( const QString &layerName, const QPointF &point );//rauno 09032013 removed: not used in calculation

public Q_SLOTS:
    void start();

public slots:

    void executeDataQuery();
    void receiveData(QVector<QPointF> route, QPointF startpoint);

Q_SIGNALS:
    void emitLaylines(QVector<QPointF> layLines);
    void finished();
    void dataQuery();

private:

    bool checkGeometriesIntersection( const QString &object1, const QString &object2 );
    bool checkIntersection( const QString &layerName, const QString &object, QString shape );
    bool checkIntersection( const QString &layerName, const QPolygonF &triangle, const QPolygonF &rhomboid );
    bool checkIntersection( const QString &layerName, const QLineF &heading, const QPolygonF &rhomboid );
    bool checkIntersection( const QString &layerName, const QLineF &heading );
    bool checkIntersection( const QString &layerName, const QPointF &point );
    bool checkIntersection( const QLineF &line1, const QLineF &line2 );
    bool checkOffset( const QPolygonF &last_triangle, const QPolygonF &present_triangle,
                      const QPointF &destiny, const float &offset);
    bool checkOffset_OnePointVersion( const QPolygonF &last_triangle,
                                      const QPolygonF &present_triangle, const float &offset);
    bool checkOffset( const QLineF &l1, const QLineF &l2, const float &offset);
    void getPath( const bool &side, const float &offset, const int &max_turns,
                  const double &windAngle, const double &layLinesAngle,
                  const QPointF &boatPos, const QPointF &destinyPos,
                  const QPolygonF &obstacles_shape, QVector<QPointF> &Path);
    QPointF getNextPoint( const QVector<QPointF> &route, const QPointF &position, const float &offset);
    int getNearestPoint( const QVector<QPointF> &route, const QPointF &boatPos);

    void openPostgreConnection();
    void updateCheckPoint();
    void updateLaylines();
    void startCalc();
    void closepostgreConnection();

    PGresult *res;
    PGconn *conn;

    // values coming from the settings
    float ACCU_OFFSET, MAX_TURNING_POINTS;

    // the data to be processed
    QPointF geoBoatPos;
    float trueWindDirection, windSpeed;
    PolarDiagram * pPolarDiagram;

    // the data processed
    QVector<QPointF> * pLeftPath, * pRightPath;
    QPointF geoDestinyPos, destinyPos;
    float layLinesAngle;
    QVector<QPointF> pathPoints;
    QPointF startPoint;
    QVector<QPointF> layLines;

    bool calculationOnGoing;
    bool obstacleFound;


    friend class bm_calculateLaylines;
};

#endif // CALCULATELAYLINES_H
