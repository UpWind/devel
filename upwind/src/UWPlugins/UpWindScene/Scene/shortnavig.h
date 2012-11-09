/**
 * \file uwshort.h
 * \author Carles Escrig i Royo
 *
 * Copyright (C) 2010 by University of Oulu (Finland)
 *
 * This file is part of UpWind Project.
 *
 * UwShort library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UwShort library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UwShort library.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef SHORTNAVIG_H
#define SHORTNAVIG_H

#include <QPolygonF>
#include <QPointF>
#include <QString>
#include <QLineF>
#include <QVector>

#include "../ChartProviderInterface/chartobjectinterface.h"

#include "/usr/include/postgresql/libpq-fe.h"
//#include "../settings/SettingsManager.h"
//#include "../shared/uwstatus.h"
#include "../shared/uwmath.h"
#include "polardiagram.h"
//#include <limits>
#include "../../UWCore/pluginmanager.h"

/// point precision to print the WKT geometries
#define WKT_P 13

//class ShortNavigation : public QThread
//{
//    Q_OBJECT
class ShortNavigation
{
public:
    static ShortNavigation* getInstance();  //Singleton class
    ShortNavigation();//ShortNavigation(QObject * parent = 0);
    ~ShortNavigation();

     void clear();

    /**
    * Generate the obstacles tables, without them the class does not work
    */

    void loadChartObjects(QVector<ChartObjectInterface*> cObjects);

    QVector<QPointF> startCalc(QPolygonF routepoints, QPointF start); //called from RouteWidget when layLine data is asked from *this

    void processData( const QVector<QPointF> * geoRoute,
                      QPointF &geoBoatPos, float &twd, float &wspeed,
                      PolarDiagram * pd, QThread::Priority = QThread::InheritPriority);

    static QPointF getFromWKTPoint( QString wkt_geometry);
    static QString buildWKTPolygon( const QPolygonF &rhomboid );
    static QString buildWKTPolygon( const QPolygonF &polygon, const QPointF &centroid,
                                    const double &perimeter, const float &offset );
    static QString buildWKTPolygon( const QPointF &point, const float &offset );
    static QString buildWKTLine( const QPointF &p1, const QPointF &p2 );
    static QString buildWKTPoint( const QPointF &p1 );
    void setPolarDiagram(PolarDiagram *diagram);

    bool publicCheckIntersection( const QString &layerName, const QPointF &point );


//protected:
//   void run();

//signals:
//    void finishedProcessing( QPointF geoDestinyPos, QPointF destinyPos,
//                             QVector<QPointF> * pLeftPath, QVector<QPointF> * pRightPath,
//                             float layLinesAngle );



private:

    static ShortNavigation *instance;

    // local values for uws
    QVector<QPolygonF > polyObstacles;
    QVector<QPointF > pointObstacles;
    QVector<QLineF > lineObstacles;
    QVector<ChartObjectInterface*> chartObjects;
    QVector<QPolygonF> rock;
    QVector<QPolygonF> wreck;
    QVector<QPolygonF> signsound;
    QVector<QPolygonF> offset;
    QList<QPoint> listqpoint;
    QPointF boatPosition;
    QPointF boatGeoPosition;



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
    bool createObstaclesTables();

    void updateCheckPoint();
    void updateLayLines();

    PGresult *res;
    PGconn *conn;

    bool debug, new_data, obstaclesTablesCreated, status;
    //SettingsManager * settingsManager;
    //UwStatus * status;
//    QMutex uws_mutex;
//    QByteArray driver;

    // values coming from the settings
    float ACCU_OFFSET, MAX_TURNING_POINTS;

    // the data to be processed
    const QVector<QPointF> * pGeoRoute;
    QPointF geoBoatPos;
    float trueWindDirection, windSpeed;
    PolarDiagram * pPolarDiagram;

    // the data processed
    QVector<QPointF> * pLeftPath, * pRightPath;
    QPointF geoDestinyPos, destinyPos;
    float layLinesAngle;
    QVector<QPointF> pathPoints;



};

#endif
