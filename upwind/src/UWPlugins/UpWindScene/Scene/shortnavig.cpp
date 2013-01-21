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

#include "shortnavig.h"
#include <ogrsf_frmts.h>
#include <QDebug>
#include <QVectorIterator>

/// Offset value factor aplied to this obstacles:
#define POINT_OFFSET_FACTOR_ROCK 10
#define POINT_OFFSET_FACTOR_SIGNSOUND 15

ShortNavigation* ShortNavigation::instance = NULL;

ShortNavigation* ShortNavigation::getInstance(){
    if(instance == NULL) {
        instance = new ShortNavigation();
        return instance;
    } else
        return instance;
}
ShortNavigation::ShortNavigation(){

    qDebug() << __PRETTY_FUNCTION__;
    //something todo
//    const char *connInfo = "";
    // conn = PQconnectStart(connInfo);

    /*
PGconn *PQsetdbLogin(const char *pghost,
                     const char *pgport,
                     const char *pgoptions,
                     const char *pgtty,
                     const char *dbName,
                     const char *login,
                     const char *pwd);

    */

    conn = PQconnectdb("hostaddr = '192.168.56.101' port = '5432' dbname = 'chart57' user = 'postgres' password = 'upwind'");

    if (PQstatus(conn) != CONNECTION_OK){
        qDebug() << "Connection not ok: " << PQerrorMessage(conn);
    }

}

ShortNavigation::~ShortNavigation()
{
    if ( instance !=NULL){
        clear();
        instance=NULL;
    }

    // THIS IS DANGEROUS IS ANOTHER UPWIND IS RUNNING IN THE SAME DB!!
//    QString sql = "DROP TABLE obstacles_r, obstacles_l;";
//    res = PQexec(conn, sql.toAscii() );
//    PQclear(res);
    PQfinish(conn);
}

void ShortNavigation::loadChartObjects(QVector<ChartObjectInterface*> cObjects) {
    this->chartObjects = cObjects;
    createObstaclesTables();

}



bool ShortNavigation::createObstaclesTables()
{
    //    OGRLayer* layer;
    status = true;
    if ( !status==false ) {
        qDebug()<<"bool ShortNavigation::createObstaclesTables()";

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
            //        qDebug() << "chartObjects size" << chartObjects.size();

            if(this->chartObjects.at(i)->getTableName() == "generarea_r") {
                for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) {
                    this->polyObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j));
                }
            }
            if(this->chartObjects.at(i)->getTableName() == "depthcont_l") {
                for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) { //TODO: parse line from polygon
                    //this->lineObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j)); //Not used in Shortnavigation -> can be removed!
                }
            }
            if(this->chartObjects.at(i)->getTableName() == "rock_p") {
                for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) { //need to check how vectors are written from DB. is getfeaturecount = polygonvector.size ?
                    for(int k = 0; k < this->chartObjects.at(i)->getCoordinateGeometry().at(j).size();k++) {
                        this->pointObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j).at(k));
                    }
                }
            }
            if(this->chartObjects.at(i)->getTableName() == "wreck_p") {
                for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) {
                    for(int k = 0; k < this->chartObjects.at(i)->getCoordinateGeometry().at(j).size();k++) {
                        this->pointObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j).at(k));
                    }
                }
            }
            if(this->chartObjects.at(i)->getTableName() == "navaid_p") {
                for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) {
                    for(int k = 0; k < this->chartObjects.at(i)->getCoordinateGeometry().at(j).size();k++) {
                        this->pointObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j).at(k));

                    }
                }
            }
            if(this->chartObjects.at(i)->getTableName() == "signsound_p") {

                QVector<QPolygonF> qpoint;
                QList<QPointF> listqpoint;
                OGRLayer* layer;
                OGRFeatureDefn *poFDefn;
                OGRFeature *poFeature;

                qpoint = this->chartObjects.at(i)->getCoordinateGeometry();
                for(int n = 0; n < qpoint.size(); n++){
                    listqpoint = qpoint.value(n).toList();
                }

                layer = this->chartObjects.at(i)->getFeatureData();
                layer->ResetReading();

                QString /*sql("SELECT * FROM *"); //FIX THIS! //*/ sql("SELECT * FROM ( SELECT DISTINCT Intersects( wkb_geometry, ");
                for(int m= 0; m < layer->GetFeatureCount();m++ ) {
                    poFDefn = layer->GetLayerDefn();
                    poFeature = layer->GetNextFeature();
                    for(int j = 0; j < poFDefn->GetFieldCount(); j++ ){
                        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( j );
                        QString str = poFieldDefn->GetNameRef();
                        if(str.contains("depth") == true) {
                            if (poFeature->GetFieldAsDouble(j) < SIGNSOUND_THRESHOLD) { //parse out depths above signsound threshold since they are not obstacles
                                pointObstacles.append(listqpoint.at(m));
                            }
                        }
                    }
                }


                //            // CREATE NEW TABLES
                sql = "CREATE TABLE obstacles_r (ogc_fid serial);";
                sql.append( "CREATE TABLE obstacles_l (ogc_fid serial);" );
                sql.append( "SELECT AddGeometryColumn ('obstacles_r','wkb_geometry',-1,'POLYGON',2);" );
                sql.append( "SELECT AddGeometryColumn ('obstacles_l','wkb_geometry',-1,'LINESTRING',2);" );
                sql.append( "ALTER TABLE obstacles_r ADD COLUMN source_table char(11);" );
                sql.append( "ALTER TABLE obstacles_l ADD COLUMN source_table char(11);" );
                res = PQexec(conn, sql.toAscii() );
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
                res = PQexec(conn, sql.toAscii() );
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
                    res = PQexec(conn, sql.toAscii() );
                    if (debug) qDebug() << QString("UwShort: Adding obstacles of %1 (%2/%3): %4 obstacles").arg( point_layers[i], QString::number(i+1), QString::number(point_layers.size()), QString::number(PQntuples(res)));
                    sql.clear();

                    for ( int j = 0; j < PQntuples(res); j++ ) {
                        QPointF point( QString( PQgetvalue( res, j, 0)).toDouble(), QString( PQgetvalue( res, j, 1)).toDouble()); //forms a qpoint double precision
                        sql.append( "INSERT INTO obstacles_r ( wkb_geometry, source_table) VALUES ( ");
                        if ( point_layers[i] == "rock_p")
                            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                        else if ( point_layers[i] == "wreck_p" )
                            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                        else if ( signsound)
                            // for signsound POINT_OFFSET = POINT_OFFSET / ( depth of signsound / factor )
                            // the bigger the signsound is, the bigger the point offset gets
                            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET / ( QString( PQgetvalue( res, j, 2)).toDouble() / POINT_OFFSET_FACTOR_SIGNSOUND ) ));
                        else
                            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET));
                        sql.append( ", '");
                        sql.append( point_layers[i]);
                        sql.append( "'); ");

                    }
                    PQclear(res);

                    res = PQexec( conn, sql.toAscii());
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
                        res = PQexec(conn, sql.toAscii() );
                        PQclear(res);
                    }


                    //  for loop for testing
                    //                for (int s=0; s<pointObstacles.size();s++){
                    //                    qDebug() << " index: " << s << "point x" << pointObstacles.at(s).x() << "y" << pointObstacles.at(s).y();
                    //                }

                    // tables are done
                    this->obstaclesTablesCreated = true;
                    qDebug("createObstaclesTables() returns = true");
                    return true;
                }

            }

        }
    }else {
        // tables are not done
        this->obstaclesTablesCreated = false;
        return false;

    }

}
QString ShortNavigation::buildWKTPolygon( const QPointF &epoint, const float &offset ) {

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


QString ShortNavigation::buildWKTPolygon( const QPolygonF &rhomboid ) {


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

QString ShortNavigation::buildWKTPolygon( const QPolygonF &epolygon, const QPointF &ecentroid,
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
