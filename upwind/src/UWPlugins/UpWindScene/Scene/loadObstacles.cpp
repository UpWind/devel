/**
 * \file uwshort.cpp
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
 */

/*
AT LATITUDE 60 DEGREES (NORTH OR SOUTH)

One degree of latitude =  111.41 km or  69.23 mi
One minute of latitude =    1.86 km or   1.15 mi
One second of latitude =   30.95 m  or 101.54 ft

One degree of longitude =  55.80 km or  34.67 mi
One minute of longitude =    .93 km or    .58 mi
One second of longitude =  15.50 m  or  50.85 ft


*/

//HOX!! MUISTA KÄYDÄ KOMMENTOIMASSA COREUPWINSCENE:CPP/PARSENMEA STRINGI TAKAISIN KÄYTTÖÖN!!!

#include "loadObstacles.h"
#include <ogrsf_frmts.h>
#include <QDebug>
#include <QVectorIterator>

/// Offset value factor aplied to this obstacles:
#define POINT_OFFSET_FACTOR_ROCK 10
#define POINT_OFFSET_FACTOR_SIGNSOUND 15

LoadObstacles* LoadObstacles::instance = NULL;

LoadObstacles* LoadObstacles::getInstance(){
    if(instance == NULL) {
        instance = new LoadObstacles();
        return instance;
    } else
        return instance;
}

LoadObstacles::LoadObstacles(){
    qDebug() << __PRETTY_FUNCTION__;
}

LoadObstacles::~LoadObstacles()
{
    if ( instance !=NULL){
        clear();
        instance=NULL;
    }

    // THIS IS DANGEROUS IS ANOTHER UPWIND IS RUNNING IN THE SAME DB!!
//    QString sql = "DROP TABLE obstacles_r, obstacles_l;";
//    res = PQexec(conn, sql.toAscii() );
//    PQclear(res);
}

void LoadObstacles::clear()
{
    // TODO NOTE this was missing
}

void LoadObstacles::loadChartObjects(QVector<ChartObjectInterface*> cObjects) {
    this->chartObjects = cObjects;
    initializeObstaclesTables();
}

bool LoadObstacles::initializeObstaclesTables() {
    qDebug()<<"bool LoadObstacles::createObstaclesTables()";

    PGconn *connection = PQconnectdb("hostaddr = '192.168.56.101' port = '5432' dbname = 'chart26' user = 'postgres' password = 'upwind'");
    PGresult *res = NULL;
    QString sql;

    if (PQstatus(connection) != CONNECTION_OK){
        qDebug() << "Connection not ok: " << PQerrorMessage(connection);
        connection = NULL;
    }

    if (connection) {
        /// Check if obstacles tables exists
        bool exists = false;

        sql = "SELECT EXISTS (select * from information_schema.tables where table_type='BASE TABLE' and table_name='obstacles_r')";
        res = PQexec(connection, sql.toLatin1() );
        if (res && PQresultStatus(res) == PGRES_TUPLES_OK) {
            qDebug() << "value: " << PQgetvalue(res, 0, 0);
            exists = (QString("t") == (QString(PQgetvalue(res, 0, 0))));
        }
        PQclear(res);
        sql.clear();

        if (exists) {
            qDebug() << "obstacles_r table exists, do not create new obstable tables";
            PQfinish(connection);
            connection = NULL;
            return false;
        }
        qDebug() << "create new obstacle tables";
    }

    /// POINT_OFFSET is the area around the obstacle,
    /// so is it's value is 5, it will draw 10 m^2

    //float POINT_OFFSET = settingsManager->getPOffset();
    float POINT_OFFSET = 5; //test values, above comment from old code
    //float NOT_ALLOWED_DEPTH = settingsManager->getMinDepth();
    float NOT_ALLOWED_DEPTH = 2; //test values, above comment from old code

    /// signsound with depth from 0 to this value are
    /// taken in consideration for the obstacles layer

    //float SIGNSOUND_THRESHOLD = settingsManager->getSignsound();
    float SIGNSOUND_THRESHOLD = 1; //test values, above comment from old code

    // ##################################################
    // list of polygon layers considered obstacles
    QList<QString> polygon_layers;
    polygon_layers.append("generarea_r");
    // list of point layers to be considered as obstacles
    QList<QString> point_layers;
    point_layers.append("rock_p");
    point_layers.append("wreck_p");
    point_layers.append("navaid_p");
    point_layers.append("signsound_p");
    // line layers to be considered as obstacles
    QList<QString> line_layers;
    line_layers.append("depthcont_l");
    //####################################################

    for (int i = 0 ; i < this->chartObjects.size(); i++) {
        if(this->chartObjects.at(i)->getTableName() == "signsound_p") {
            // CREATE NEW TABLES
            sql = "CREATE TABLE obstacles_r (ogc_fid serial);";
            sql.append( "CREATE TABLE obstacles_l (ogc_fid serial);" );
            sql.append( "SELECT AddGeometryColumn ('obstacles_r','wkb_geometry',-1,'POLYGON',2);" );
            sql.append( "SELECT AddGeometryColumn ('obstacles_l','wkb_geometry',-1,'LINESTRING',2);" );
            sql.append( "ALTER TABLE obstacles_r ADD COLUMN source_table char(11);" );
            sql.append( "ALTER TABLE obstacles_l ADD COLUMN source_table char(11);" );
            res = PQexec(connection, sql.toLatin1() );
            PQclear(res);

            // INSERT POLYGON LAYERS
            sql.clear();
            // DIRECT VERSION, WITHOUT ADDING THE OFFSET

            for ( int i = 0; i < polygon_layers.size(); i++ ) {
                sql.append( "INSERT INTO obstacles_r( wkb_geometry, source_table) SELECT wkb_geometry, '");
                sql.append( polygon_layers[i] );
                sql.append( "' AS source_table FROM " );
                sql.append( polygon_layers[i] );
                sql.append( ";" );
                if (debug) qDebug() << QString("UwShort: Adding obstacles of %1").arg( polygon_layers[i]);
            }
            // WARNING: very long string:
            res = PQexec(connection, sql.toLatin1() );
            PQclear(res);

            qDebug() << "for in 1";            // INSERT POINT LAYERS WITH OFFSET
            for ( int i = 0; i < point_layers.size(); i++ ) {
                bool signsound = ( point_layers[i] == "signsound_p" );
                sql = "SELECT X(wkb_geometry),Y(wkb_geometry)";
                if ( signsound)
                    sql.append( ",depth");
                sql.append( " FROM ");
                sql.append( point_layers[i] );
                if ( signsound)
                    sql.append( QString(" WHERE depth < %1 ").arg( QString::number( SIGNSOUND_THRESHOLD)));
                sql.append( ";");
                res = PQexec(connection, sql.toLatin1() );
                if (debug) qDebug() << QString("UwShort: Adding obstacles of %1 (%2/%3): %4 obstacles").arg( point_layers[i], QString::number(i+1), QString::number(point_layers.size()), QString::number(PQntuples(res)));
                sql.clear();

                for ( int j = 0; j < PQntuples(res); j++ ) {
                    QPointF point( QString( PQgetvalue( res, j, 0)).toDouble(), QString( PQgetvalue( res, j, 1)).toDouble()); //forms a qpoint double precision
                    sql.append( "INSERT INTO obstacles_r ( wkb_geometry, source_table) VALUES ( ");
                    if ( point_layers[i] == "rock_p")
                        sql.append( LoadObstacles::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                    else if ( point_layers[i] == "wreck_p" )
                        sql.append( LoadObstacles::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                    else if ( signsound)
                        // for signsound POINT_OFFSET = POINT_OFFSET / ( depth of signsound / factor )
                        // the bigger the signsound is, the bigger the point offset gets
                        sql.append( LoadObstacles::buildWKTPolygon( point, POINT_OFFSET / ( QString( PQgetvalue( res, j, 2)).toDouble() / POINT_OFFSET_FACTOR_SIGNSOUND ) ));
                    else
                        sql.append( LoadObstacles::buildWKTPolygon( point, POINT_OFFSET));
                    sql.append( ", '");
                    sql.append( point_layers[i]);
                    sql.append( "'); ");

                }
                PQclear(res);

                res = PQexec(connection, sql.toLatin1());
                PQclear(res);

                //INSERT LINE LAYERS
                sql.clear();
                for ( int i = 0; i < line_layers.size(); i++ ) {
                    sql.append( "INSERT INTO obstacles_l( wkb_geometry, source_table) SELECT wkb_geometry, '");
                    sql.append( line_layers[i]);
                    sql.append( "' AS source_table FROM " );
                    sql.append( line_layers[i] );
                    if ( line_layers[i] == "depthcont_l" ) {
                        sql.append( " WHERE valdco<=" );
                        sql.append( QString::number( NOT_ALLOWED_DEPTH));
                    }
                    sql.append( ";");
                    if (debug) {
                        // WARNING: very long string:
                        qDebug() << QString("UwShort: Adding obstacles of ").append( line_layers[i]);
                    }
                    res = PQexec(connection, sql.toLatin1());
                    PQclear(res);
                }

                // tables are done
                qDebug("createObstaclesTables() returns = true");
                return true;
            }
        }
    }
    if (connection) {
        PQfinish(connection);
    }
    return true;
}

QString LoadObstacles::buildWKTPolygon( const QPointF &epoint, const float &offset ) {

    QPointF point = epoint;
    UwMath::toConformal( point);
    double moffset = offset / ( cos( UwMath::toRadians(point.y())) * 6378000.0 );

    QString WKTPolygon = "'POLYGON((";
    WKTPolygon.append( QString::number( UwMath::toDegrees( point.x() + moffset), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( point.y() + moffset), 'f', WKT_P));
    WKTPolygon.append( ",");
    WKTPolygon.append(  QString::number( UwMath::toDegrees( point.x() + moffset), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( point.y() - moffset), 'f', WKT_P));
    WKTPolygon.append( ",");
    WKTPolygon.append( QString::number( UwMath::toDegrees( point.x() - moffset), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( point.y() - moffset), 'f', WKT_P));
    WKTPolygon.append( ",");
    WKTPolygon.append( QString::number( UwMath::toDegrees( point.x() - moffset), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( point.y() + moffset), 'f', WKT_P));
    WKTPolygon.append( ",");
    WKTPolygon.append( QString::number( UwMath::toDegrees( point.x() + moffset), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( point.y() + moffset), 'f', WKT_P));
    WKTPolygon.append( "))'");

    return WKTPolygon;
}


QString LoadObstacles::buildWKTPolygon( const QPolygonF &rhomboid ) {


    QString WKTPolygon = "'POLYGON((";
    for ( int i = 0; i < rhomboid.size(); i++ ) {
        WKTPolygon.append( QString::number( rhomboid[i].x(), 'f', WKT_P));
        WKTPolygon.append( " ");
        WKTPolygon.append( QString::number( rhomboid[i].y(), 'f', WKT_P));
        WKTPolygon.append( ",");
    }
    WKTPolygon.append( QString::number( rhomboid.at(0).x(), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( rhomboid.at(0).y(), 'f', WKT_P));
    WKTPolygon.append( "))'");

    return WKTPolygon;
}

QString LoadObstacles::buildWKTPolygon( const QPolygonF &epolygon, const QPointF &ecentroid,
                                          const double &perimeter, const float &offset )
{
    // The intention of this was to make the polygons grow but it is not as simple as we thought.
    // Take a look into skeletons to continue developing this way

    bool debug = 1;

    if (debug) {
        qDebug() << "builtWKTPolygon(): add offset to a polygon";
        QString dbg("builtWKTPolygon(): ");
        dbg.append( "polygon size: ");
        dbg.append( QString::number( epolygon.size()));
        dbg.append( " exple: (");
        dbg.append( QString::number( epolygon[0].x()));
        dbg.append( " ,");
        dbg.append( QString::number( epolygon[0].y()));
        dbg.append( ")");
        dbg.append( " centroid: (");
        dbg.append( QString::number( ecentroid.x()));
        dbg.append( " ,");
        dbg.append( QString::number( ecentroid.y()));
        dbg.append( ") perimeter: ");
        dbg.append( QString::number( perimeter));
        dbg.append( " offset: ");
        dbg.append( QString::number( offset));
        qDebug() << dbg;
    }

    QPolygonF polygon = UwMath::toConformal( epolygon);
    QPointF centroid = UwMath::toConformal( ecentroid);

    if (debug) {
        QString dbg("builtWKTPolygon(): ");
        dbg.append( "exple poly conf: (");
        dbg.append( QString::number( polygon[0].x()));
        dbg.append( " ,");
        dbg.append( QString::number( polygon[0].y()));
        dbg.append( ")");
        dbg.append( "exple cntroid conf: (");
        dbg.append( QString::number( centroid.x()));
        dbg.append( " ,");
        dbg.append( QString::number( centroid.y()));
        dbg.append( ")");
        qDebug() << dbg;
    }

    double moffset = offset / ( cos( UwMath::toRadians(centroid.y())) * 6378000.0 );

    QPointF old_point;
    double new_perimeter = perimeter + moffset;
    double factor = new_perimeter / perimeter;

    if (debug) {
        QString dbg("builtWKTPolygon(): ");
        dbg.append( "moffset: ");
        dbg.append( QString::number( moffset));
        dbg.append( " new_perimeter: ");
        dbg.append( QString::number( new_perimeter));
        dbg.append( " factor: ");
        dbg.append( QString::number( factor));
        qDebug() << dbg;
    }

    for ( int i = 0; i < polygon.size(); i++) {
        old_point = polygon[i];
        polygon[i].setX( old_point.x() + ( old_point.x() - centroid.x() ) * factor );
        polygon[i].setY( old_point.y() + ( old_point.y() - centroid.y() ) * factor );
    }

    if (debug) {
        QString dbg("builtWKTPolygon(): ");
        dbg.append( "exple output: (");
        dbg.append( QString::number( polygon[0].x()));
        dbg.append( " ,");
        dbg.append( QString::number( polygon[0].y()));
        dbg.append( ")");
        qDebug() << dbg;
    }

    QString WKTPolygon = "'POLYGON((";
    for ( int i = 0; i < polygon.size(); i++ ) {
        WKTPolygon.append( QString::number( UwMath::toDegrees( polygon[i].x()), 'f', WKT_P));
        WKTPolygon.append( " ");
        WKTPolygon.append( QString::number( UwMath::fromMercator( polygon[i].y()), 'f', WKT_P));
        WKTPolygon.append( ",");
    }
    WKTPolygon.append( QString::number( UwMath::toDegrees( polygon[0].x()), 'f', WKT_P));
    WKTPolygon.append( " ");
    WKTPolygon.append( QString::number( UwMath::fromMercator( polygon[0].y()), 'f', WKT_P));
    WKTPolygon.append( "))'");

    return WKTPolygon;
}
