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

#include "../ChartProviderInterface/chartobjectinterface.h"

#include "libpq-fe.h"
#include "../shared/uwmath.h"
#include "polardiagram.h"
#include "../../UWCore/pluginmanager.h"

/// point precision to print the WKT geometries
#define WKT_P 13

class LoadObstacles
{
public:
    static LoadObstacles* getInstance();  //Singleton class
    LoadObstacles();
    ~LoadObstacles();

    void clear();

    /**
    * Generate the obstacles tables, without them the class does not work
    */

    void loadChartObjects(QVector<ChartObjectInterface*> cObjects);
    PGresult *res;
    PGconn *conn;

private:
    static QString buildWKTPolygon( const QPolygonF &rhomboid );
    static QString buildWKTPolygon( const QPolygonF &polygon, const QPointF &centroid,
                                    const double &perimeter, const float &offset );
    static QString buildWKTPolygon( const QPointF &point, const float &offset );
    QVector<QPolygonF > polyObstacles;
    QVector<QPointF > pointObstacles;
    QVector<QLineF > lineObstacles;
    QVector<ChartObjectInterface*> chartObjects;
    QVector<QPolygonF> rock;
    QVector<QPolygonF> wreck;
    QVector<QPolygonF> signsound;
    QVector<QPolygonF> offset;
    QList<QPoint> listqpoint;
    static LoadObstacles *instance;
    bool createObstaclesTables();

    bool debug, new_data, obstaclesTablesCreated, status;

};

#endif
