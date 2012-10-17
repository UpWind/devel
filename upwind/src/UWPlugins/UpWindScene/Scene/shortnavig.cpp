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



#include "shortnavig.h"
#include <ogrsf_frmts.h>
#include <QDebug>

/// Offset value factor aplied to this obstacles:
#define POINT_OFFSET_FACTOR_ROCK 10
#define POINT_OFFSET_FACTOR_SIGNSOUND 15

ShortNavigation::ShortNavigation( QObject * parent)
    : QThread(parent)
{
    //this->settingsManager = SettingsManager::getInstance();
    //status  = UwStatus::getInstance();
    //debug = status->debug();

    // In the settings the accuracy to calculate the turning points
    // is in meters, but we want it in km for the function

    //this->ACCU_OFFSET = settingsManager->getAccu() / 1000;
    //this->MAX_TURNING_POINTS = settingsManager->getMaxTurn();

    //this->new_data = false;



    this->start();
}

ShortNavigation* ShortNavigation::instance = NULL;

ShortNavigation* ShortNavigation::getInstance(){
    if(instance == NULL) {
        instance = new ShortNavigation();
        qDebug() << __PRETTY_FUNCTION__;
        return instance;
    } else
        return instance;
}

ShortNavigation::~ShortNavigation()
{
    //this->obstacles.resize(0);

    // THIS IS DANGEROUS IS ANOTHER UPWIND IS RUNING IN THE SAME DB!!
    //QString sql = "DROP TABLE obstacles_r, obstacles_l;";
    //res = PQexec(conn, sql.toAscii() );
    //PQclear(res);
}

void ShortNavigation::loadChartObjects(QVector<ChartObjectInterface*> cObjects) {
    this->chartObjects = cObjects;
    //createObstaclesTables();
}


bool ShortNavigation::createObstaclesTables()
{
    //if ( !status->obstaclesTablesCreated() ) {





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

    //if (!this->obstacles.isEmpty()) this->obstacles.resize(0);



    for (int i = 0 ; i < this->chartObjects.size(); i++) {

        if(this->chartObjects.at(i)->getTableName() == "generarea_l") {
            for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) {
                this->polyObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j));
            }
        }
        if(this->chartObjects.at(i)->getTableName() == "depthcont_l") {
            for (int j = 0 ; j < this->chartObjects.at(i)->getCoordinateGeometry().size();j++) { //TODO: parse line from polygon
                //this->lineObstacles.append(this->chartObjects.at(i)->getCoordinateGeometry().at(j));
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
            OGRLayer* layer;
            OGRFeatureDefn *poFDefn;
            OGRFeature *poFeature;

            qpoint = this->chartObjects.at(i)->getCoordinateGeometry();
            layer = this->chartObjects.at(i)->getFeatureData();
            layer->ResetReading();

            for(int m= 0; m < layer->GetFeatureCount();m++ ) {

                poFDefn = layer->GetLayerDefn();
                poFeature = layer->GetNextFeature();

                for(int j = 0; j < poFDefn->GetFieldCount(); j++ ){
                    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( j );
                    QString str = poFieldDefn->GetNameRef() ;

                    if(str.contains("depth") == true) {
                        if (poFeature->GetFieldAsDouble(j) > SIGNSOUND_THRESHOLD) { //parse out depths above signsound threshold as since they are not obstacles
                            qpoint.remove(j);
                        }
                    }
                }
            }

            //for (int j = 0 ; j < qpoint.at(i)->getCoordinateGeometry().size();j++) {
            //     for(int k = 0; k < qpoint.at(i)->getCoordinateGeometry().at(j).size();k++) {
            //         this->pointObstacles.append(qpoint.at(i)->getCoordinateGeometry().at(j).at(k));
            //     }
            // }



        }

    }
}
/*
            for (int l = 0; l < this->pointObstacles.size();l++) {
                if ( point_layers[i] == "rock_p")
                    sql.append( UwShort::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                else if ( point_layers[i] == "wreck_p" )
                    sql.append( UwShort::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
                else if ( signsound)
                    // for signsound POINT_OFFSET = POINT_OFFSET / ( depth of signsound / factor )
                    // the bigger the signsound is, the bigger the point offset gets
                    sql.append( UwShort::buildWKTPolygon( point, POINT_OFFSET / ( QString( PQgetvalue( res, j, 2)).toDouble() / POINT_OFFSET_FACTOR_SIGNSOUND ) ));
                else
                    sql.append( UwShort::buildWKTPolygon( point, POINT_OFFSET));
            }
//} */



//// CREATE NEW TABLES
//sql = "CREATE TABLE obstacles_r (ogc_fid serial);";
//sql.append( "CREATE TABLE obstacles_l (ogc_fid serial);" );
//sql.append( "SELECT AddGeometryColumn ('obstacles_r','wkb_geometry',-1,'POLYGON',2);" );
//sql.append( "SELECT AddGeometryColumn ('obstacles_l','wkb_geometry',-1,'LINESTRING',2);" );
//sql.append( "ALTER TABLE obstacles_r ADD COLUMN source_table char(11);" );
//sql.append( "ALTER TABLE obstacles_l ADD COLUMN source_table char(11);" );
//res = PQexec(conn, sql.toAscii() );
//PQclear(res);

//// INSERT POLYGON LAYERS
//sql.clear();
//// DIRECT VERSION, WITHOUT ADDING THE OFFSET
//for ( int i = 0; i < polygon_layers.size(); i++ ) {
//    sql.append( "INSERT INTO obstacles_r( wkb_geometry, source_table) SELECT wkb_geometry, '");
//    sql.append( polygon_layers[i] );
//    sql.append( "' AS source_table FROM " );
//    sql.append( polygon_layers[i] );
//    sql.append( ";" );
//    if (debug) qDebug() << QString("UwShort: Adding obstacles of %1").arg( polygon_layers[i]);
//}
//// WARNING: very long string:

//res = PQexec(conn, sql.toAscii() );
//PQclear(res);


//// INSERT POINT LAYERS WITH OFFSET
//for ( int i = 0; i < point_layers.size(); i++ ) {
//    bool signsound = ( point_layers[i] == "signsound_p" );
//    sql = "SELECT X(wkb_geometry),Y(wkb_geometry)";
//    if ( signsound)
//        sql.append( ",depth");
//    sql.append( " FROM ");
//    sql.append( point_layers[i] );
//    if ( signsound)
//        sql.append( QString(" WHERE depth < %1 ").arg( QString::number( SIGNSOUND_THRESHOLD)));
//    sql.append( ";");
//    res = PQexec(conn, sql.toAscii() );
//    if (debug) qDebug() << QString("UwShort: Adding obstacles of %1 (%2/%3): %4 obstacles").arg( point_layers[i], QString::number(i+1), QString::number(point_layers.size()), QString::number(PQntuples(res)));
//    sql.clear();

//    for ( int j = 0; j < PQntuples(res); j++ ) {
//        QPointF point( QString( PQgetvalue( res, j, 0)).toDouble(), QString( PQgetvalue( res, j, 1)).toDouble()); //forms a qpoint double precision
//        sql.append( "INSERT INTO obstacles_r ( wkb_geometry, source_table) VALUES ( ");
//        if ( point_layers[i] == "rock_p")
//            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
//        else if ( point_layers[i] == "wreck_p" )
//            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET * POINT_OFFSET_FACTOR_ROCK ));
//        else if ( signsound)
//            // for signsound POINT_OFFSET = POINT_OFFSET / ( depth of signsound / factor )
//            // the bigger the signsound is, the bigger the point offset gets
//            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET / ( QString( PQgetvalue( res, j, 2)).toDouble() / POINT_OFFSET_FACTOR_SIGNSOUND ) ));
//        else
//            sql.append( ShortNavigation::buildWKTPolygon( point, POINT_OFFSET));
//        sql.append( ", '");
//        sql.append( point_layers[i]);
//        sql.append( "'); ");

//    }
//    PQclear(res);

//    res = PQexec( conn, sql.toAscii());
//    PQclear(res);
//}

//// INSERT LINE LAYERS
//sql.clear();
//for ( int i = 0; i < line_layers.size(); i++ ) {
//    sql.append( "INSERT INTO obstacles_l( wkb_geometry, source_table) SELECT wkb_geometry, '");
//    sql.append( line_layers[i]);
//    sql.append( "' AS source_table FROM " );
//    sql.append( line_layers[i] );
//    if ( line_layers[i] == "depthcont_l" ) {
//        sql.append( " WHERE valdco<=" );
//        sql.append( QString::number( NOT_ALLOWED_DEPTH));
//    }
//    sql.append( ";");
//    if (debug) {
//        // WARNING: very long string:
//        //qDebug() << sql;
//        qDebug() << QString("UwShort: Adding obstacles of ").append( line_layers[i]);
//    }
//    res = PQexec(conn, sql.toAscii() );
//    PQclear(res);
//}




////status->setObstaclesTablesCreated( true); //uwstatus

//// tables are done
//return true;

//} else {


//// tables are not done
//return false;

//}


//}

QPointF ShortNavigation::getFromWKTPoint( QString wkt_geometry)
{
    // POINT(25.0346075555951 65.2062704150903)

    // remove the end and start
    wkt_geometry.remove( wkt_geometry.size() - 1, 1);
    wkt_geometry.remove( 0, 6);

    return QPointF( wkt_geometry.section(" ",0,0).toDouble(),
                    wkt_geometry.section(" ",1,1).toDouble() );
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
    // THIS WAS INTENDED TO MAKE THE POLYGONS GROW, BY IS NOT AS SIMPLE...
    // Take a look into skeletons to continue this way

    bool debug = 1;

    if (debug) {
        qDebug() << "UwShort::builtWKTPolygon(): add offset to a polygon";
        QString dbg("UwShort::builtWKTPolygon(): ");
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
        QString dbg("UwShort::builtWKTPolygon(): ");
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
        QString dbg("UwShort::builtWKTPolygon(): ");
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
        QString dbg("UwShort::builtWKTPolygon(): ");
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

QString ShortNavigation::buildWKTLine( const QPointF &p1, const QPointF &p2 ) {

    QString WKTLine = "'LINESTRING(";
    WKTLine.append( QString::number( p1.x(), 'f', WKT_P));
    WKTLine.append( " ");
    WKTLine.append( QString::number( p1.y(), 'f', WKT_P));
    WKTLine.append( ",");
    WKTLine.append( QString::number( p2.x(), 'f', WKT_P));
    WKTLine.append( " ");
    WKTLine.append( QString::number( p2.y(), 'f', WKT_P));
    WKTLine.append( ")'");

    return WKTLine;
}

QString ShortNavigation::buildWKTPoint( const QPointF &p1 ) {

    QString WKTPoint = "'POINT(";
    WKTPoint.append( QString::number( p1.x(), 'f', WKT_P));
    WKTPoint.append( " ");
    WKTPoint.append( QString::number( p1.y(), 'f', WKT_P));
    WKTPoint.append( ")'");

    return WKTPoint;
}

bool ShortNavigation::checkGeometriesIntersection( const QString &object1, const QString &object2 ) {

    QString sql("SELECT * FROM ( SELECT DISTINCT Intersects( ");
    sql.append( object1);
    sql.append( ", ");
    sql.append( object2);
    sql.append( ") AS result ");
    sql.append( " ) AS intersection WHERE result = TRUE");

    res = PQexec(conn, sql.toAscii() );
    bool intersection = ( PQntuples(res) > 0 );
    PQclear(res);

    return intersection;
}

bool ShortNavigation::checkIntersection( const QString &layerName, const QString &object, QString shape = QString() ) {

    QString sql("SELECT * FROM ( SELECT DISTINCT Intersects( wkb_geometry, ");
    sql.append( object);
    sql.append( ") AS result FROM ");
    // if shape is empty string, then there is no area of rectriction
    if ( shape.isEmpty()  ) {
        sql.append( layerName);
        sql.append( " ) AS intersection ");
    } else {
        sql.append( layerName);
        sql.append( " WHERE wkb_geometry && GeometryFromText( ");
        sql.append( shape);
        sql.append( ") ) AS intersection ");
    }
    sql.append( " WHERE result = TRUE");

    res = PQexec(conn, sql.toAscii() );
    bool intersection = ( PQntuples(res) > 0 );
    PQclear(res);

    return intersection;
}

bool ShortNavigation::checkIntersection( const QString &layerName, const QPolygonF &triangle, const QPolygonF &rhomboid ) {

    QString WKTTriangle = buildWKTPolygon( triangle );
    QString WKTPolygon = buildWKTPolygon( rhomboid );

    return checkIntersection( layerName, WKTTriangle, WKTPolygon);

}

bool ShortNavigation::checkIntersection( const QString &layerName, const QPointF &point ) {

    QString WKTPoint = buildWKTPoint( point );

    return checkIntersection( layerName, WKTPoint);

}

bool ShortNavigation::publicCheckIntersection( const QString &layerName, const QPointF &point ) {

    bool self_conn = false;

    if ( !status->obstaclesTablesCreated() )
        this->createObstaclesTables();

//    if ( !this->hasConn() ) {
//        this->openConn();
//        self_conn = true;
//    }

    QString WKTPoint = buildWKTPoint( point );

    bool result = checkIntersection( layerName, WKTPoint);

//    if ( self_conn )
//        this->closeConn();

    return result;
}

bool ShortNavigation::checkIntersection( const QString &layerName, const QLineF &heading, const QPolygonF &rhomboid ) {

    QString WKTLine = buildWKTLine( heading.p1(), heading.p2() );
    QString WKTPolygon = buildWKTPolygon( rhomboid );

    return checkIntersection( layerName, WKTLine, WKTPolygon);

}

bool ShortNavigation::checkIntersection( const QString &layerName, const QLineF &heading ) {

    QString WKTLine = buildWKTLine( heading.p1(), heading.p2() );

    return checkIntersection( layerName, WKTLine);

}

bool ShortNavigation::checkIntersection( const QLineF &line1, const QLineF &line2 ) {

    QString WKTLine1 = buildWKTLine( line1.p1(), line1.p2() );
    QString WKTLine2 = buildWKTLine( line2.p1(), line2.p2() );

    return checkGeometriesIntersection( WKTLine1, WKTLine2);

}

bool ShortNavigation::checkOffset( const QPolygonF &last_triangle, const QPolygonF &present_triangle, const QPointF &destiny, const float &offset ) {

    double diff = UwMath::getDistance(last_triangle.at( 2), present_triangle.at( 2));
    double scale = ( UwMath::getDistance(present_triangle.at( 0), destiny) +
                     UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 1)) ) / 2 ;
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    //std::cout << "Diff: " << diff << " / scale: " << scale << " criteria: " << criteria << std::endl;

    return diff < criteria;
}

bool ShortNavigation::checkOffset_OnePointVersion( const QPolygonF &last_triangle, const QPolygonF &present_triangle, const float &offset ) {

    double diff = UwMath::getDistance(last_triangle.at( 2), present_triangle.at( 2));
    double scale = ( UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 2)) +
                     UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 1)) ) / 2 ;
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    //std::cout << "OnePointsVersion Diff: " << diff << " / scale: " << scale << " criteria: " << criteria << std::endl;

    return diff < criteria;
}

bool ShortNavigation::checkOffset( const QLineF &l1, const QLineF &l2, const float &offset ) {

    double diff = UwMath::getDistance(l1.p2(), l2.p2());
    double scale = ( UwMath::getDistance(l1.p1(), l1.p2()) +
                     UwMath::getDistance(l2.p1(), l2.p2()) ) / 2 ;
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    return diff < criteria;
}

void ShortNavigation::getPath( const bool &side, const float &offset, const int &max_turns,
                               const double &windAngle, const double &layLinesAngle,
                               const QPointF &boatPos, const QPointF &destinyPos,
                               const QPolygonF &obstacles_shape, QVector<QPointF> &Path )
{
    //input has to be Geographical data cause we check against PostGIS

    QPointF TPleft, TPright;
    bool sideRight = side;
    bool ready, obs_r, obs_l = false;
    int count = 0;
    // Path is returned by reference, clear
    Path.clear();
    // first point in the path is the origin
    Path.append( boatPos);
    QPolygonF present_triangle;
    QPolygonF last_triangle;


    // while the last point in the path is not the destiny
    // AND we don't exceed the maximum turning points setting
    while ( Path.last() != destinyPos && count < max_turns ) {

        // clear triangle
        present_triangle.clear();

        // get the turning points between the last point and dest
        // for the first run last point in the path is origin
        UwMath::getTurningPoints( TPleft, TPright,
                                  windAngle, layLinesAngle,
                                  Path.last(), destinyPos);

        // build a new triangle
        present_triangle << Path.last();
        if (sideRight) present_triangle << TPright;
        else present_triangle << TPleft;
        present_triangle << destinyPos;

        // check for obstacles in the triangle
        obs_r = checkIntersection( "obstacles_r", present_triangle, obstacles_shape );
        obs_l = checkIntersection( "obstacles_l", present_triangle, obstacles_shape );

        if ( obs_r || obs_l ) {

            // Obstacles found: let's reduce the triangle until they disapear
            ready = false;

            last_triangle = present_triangle;
            // reduce triangle to half
            present_triangle = UwMath::triangleToHalf( present_triangle );

            while ( !ready ) {

                // check again, put booleans first and checkIntersection will not be even called
                if ( ( obs_r && checkIntersection( "obstacles_r", present_triangle, obstacles_shape ) ) ||
                     ( obs_l && checkIntersection( "obstacles_l", present_triangle, obstacles_shape ) ) ) {

                    // Still obstacles: reduce again!
                    last_triangle = present_triangle;
                    present_triangle = UwMath::triangleToHalf( present_triangle );

                } else {

                    // No more obstacles
                    if (  checkOffset( last_triangle, present_triangle, destinyPos, offset ) ) {
                        // the distance is acceptable, finetune done
                        ready = true;
                    } else {
                        // we are too far, let's increase a bit the triangle
                        present_triangle = UwMath::avgTriangle( present_triangle, last_triangle);
                    }
                }

            }
        }

        // this triangle has no obstacles inside
        Path.append( present_triangle.at( 1 ) );
        Path.append( present_triangle.at( 2 ) );
        // add a turn
        count++;

        // we continue in the other side
        sideRight = (!sideRight);
    }
}

QPointF ShortNavigation::getNextPoint( const QVector<QPointF> &route, const QPointF &boatPos, const float &offset) {

    //input should be in GEOGRAPHICAL format

    if ( checkIntersection( "obstacles_r", boatPos ) ) {
        // if we are inside an obstacle, don't even try

        if (debug) qDebug() << "UwShort::getNextPoint(): WARNING! YOU ARE INSIDE AN OBSTACLE AREA!";

        return boatPos;
    }

    // we receive a route here, a list of points
    // let's see at what point of the route we are ...
    double distance = std::numeric_limits<double>::max();
    int nearest_point = 0;

    for ( int i = 0; i < route.size(); i++) {
        double temp = UwMath::getDistance( boatPos, route.at(i));
        if ( temp < distance ) {
            distance = temp;
            nearest_point = i;
            //                 may be in the future we don't need to go through all of them...
            //		} else if ( temp > distance ) {
            //			break;
        }
    }

    // we got the nearest point in thre route
    // but could be far, let's find out the real
    // nearest point of the route by making a
    // projection towards it...
    // START SEARCH OF PROJECTION FOR FINETUNE
    QPointF projection_point;
    QLineF route_line, projection_line;
    QPointF a, b, c;
    if ( route.size() >= 2 ) {

        if ( route.size() - nearest_point == 1 ) {
            // IF NEAREST IS THE LAST ONE
            a = UwMath::toConformal( route.at( nearest_point - 1 ) );
            b = UwMath::toConformal( route.at( nearest_point ) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point - 1 ));
            route_line.setP2( route.at( nearest_point ));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

        } else if ( nearest_point == 0 ) {
            // IF NEAREST IS THE FIRST ONE
            a = UwMath::toConformal( route.at( nearest_point) );
            b = UwMath::toConformal( route.at( nearest_point + 1) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point));
            route_line.setP2( route.at( nearest_point + 1));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

        } else {
            // IF NEAREST IS NOT FIRST OR LAST ONE
            a = UwMath::toConformal( route.at( nearest_point) );
            b = UwMath::toConformal( route.at( nearest_point + 1) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point));
            route_line.setP2( route.at( nearest_point + 1));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

            // we must check if our position is between nearest and nearest + 1
            // or between nearest - 1 and nearest
            if ( !checkIntersection( route_line, projection_line) ) {
                a = UwMath::toConformal( route.at( nearest_point - 1 ) );
                b = UwMath::toConformal( route.at( nearest_point ) );
                c = UwMath::toConformal( (const QPointF)boatPos );

                projection_point = UwMath::getProjectionPoint( a, b, c);
                UwMath::fromConformal( projection_point);

                route_line.setP1( route.at( nearest_point - 1 ));
                route_line.setP2( route.at( nearest_point ));
                projection_line.setP1( boatPos);
                projection_line.setP2( projection_point);
            }
        }
    }


    // PROCESS TO FIND THE CHECKPOINT

    if ( route.size() < 1 ) {

        if (debug) qDebug() << "UwShort::getNextPoint(): Fatal: route must have at least 1 point!";

    } else if ( route.size() == 1 ) {

        // ROUTE TO PROCESS ONLY HAS ONE POINT
        QLineF heading( boatPos, route.at( nearest_point));

        bool hobs_r = checkIntersection( "obstacles_r", heading );
        bool hobs_l = checkIntersection( "obstacles_l", heading );

        // FINETUNE CHECKPOINT IN THE HEADING
        if ( hobs_r || hobs_l ) {

            bool ready = false;

            QLineF last_heading;

            while ( !ready ) {


                if ( ( hobs_r && checkIntersection( "obstacles_r", heading ) ) ||
                     ( hobs_l && checkIntersection( "obstacles_l", heading ) ) ) {

                    last_heading = heading;
                    heading = UwMath::lineToHalf( heading);

                } else {
                    if (  checkOffset( heading, last_heading, offset ) )
                        ready = true;
                    else
                        heading = UwMath::avgLine( heading, last_heading);
                }
            }

            return heading.p2();

        } else {

            return heading.p2();
        }

    } else if ( route.size() > 1 ) {

        // ROUTE TO PROCESS HAS MORE THAN 1 POINT
        int i = nearest_point;

        QPolygonF triangle;
        // 1st POINT
        triangle << boatPos;

        // 2nd POINT
        if ( checkIntersection( route_line, projection_line) ) {
            triangle << projection_point;
        } else {
            triangle << route.at( nearest_point);
        }

        // 3rd POINT
        if ( route.at( nearest_point) == route_line.p2() ) {
            triangle << route.at( nearest_point);
            i = nearest_point - 1;
        } else if ( route.at( nearest_point) == route_line.p1() ) {
            triangle << route.at( nearest_point + 1);
        }

        // ########################################################################
        // SPECIAL CHECK to find obstacles in the heading of the first triangle  ##
        // ########################################################################
        if ( triangle.at( 0 ) == boatPos ) {
            QLineF heading( triangle.at( 0), triangle.at( 1));

            bool hobs_r = checkIntersection( "obstacles_r", heading, triangle );
            bool hobs_l = checkIntersection( "obstacles_l", heading, triangle );

            // FINETUNE CHECKPOINT IN THE HEADING
            if ( hobs_r || hobs_l ) {

                bool ready = false;

                QLineF last_heading;

                while ( !ready ) {


                    if ( ( hobs_r && checkIntersection( "obstacles_r", heading, triangle) ) ||
                         ( hobs_l && checkIntersection( "obstacles_l", heading, triangle)) ) {

                        last_heading = heading;
                        heading = UwMath::lineToHalf( heading);

                    } else {
                        if (  checkOffset( heading, last_heading, offset ) )
                            ready = true;
                        else
                            heading = UwMath::avgLine( heading, last_heading);
                    }
                }

                return heading.p2();

            }
        }
        // ########################################################################
        // END SPECIAL CHECK                                                     ##
        // ########################################################################


        bool obs_r = checkIntersection( "obstacles_r", triangle, triangle );
        bool obs_l = checkIntersection( "obstacles_l", triangle, triangle );

        while ( !obs_r && !obs_l && ( i < ( route.size() - 2 ) ) ) {

            triangle.clear();
            i++;

            triangle << boatPos;
            triangle << route.at( i);
            triangle << route.at( i + 1);

            obs_r = checkIntersection( "obstacles_r", triangle, triangle );
            obs_l = checkIntersection( "obstacles_l", triangle, triangle );
        }

        // FINETUNE CHECKPOINT

        if ( obs_r || obs_l ) {

            bool ready = false;

            QPolygonF last_triangle;

            while ( !ready ) {


                if ( ( obs_r && checkIntersection( "obstacles_r", triangle, triangle) ) ||
                     ( obs_l && checkIntersection( "obstacles_l", triangle, triangle)) ) {

                    last_triangle = triangle;
                    triangle = UwMath::triangleToHalf_OnePointVersion( triangle);

                } else {
                    if (  checkOffset_OnePointVersion( last_triangle, triangle, offset ) )
                        ready = true;
                    else
                        triangle = UwMath::avgTriangle_OnePointVersion( triangle, last_triangle);
                }
            }

        }

        return triangle.at( 2);

    }
    // END PROCESS TO FIND THE CHECKPOINT

}

void ShortNavigation::updateCheckPoint()
{
    if (debug) qDebug() << "UwShort::updateCheckPoint(): started";

    // Route has been updated by user or method
    // It is set in Conformal Inverted coordenates (Qt scene)
    // this->geoRoute = UwMath::fromConformalInverted( (const QList<QPointF>)this->route);

    // Let's find out which is the next point in our route

    // find the destiny check point in geographical format
    geoDestinyPos = this->getNextPoint( *pGeoRoute, geoBoatPos, ACCU_OFFSET);
    // and set it for Qt scene as well
    destinyPos = UwMath::toConformalInverted( (const QPointF)geoDestinyPos);

    if (debug) qDebug() << "UwShort::updateCheckPoint(): ended: ok";

}

void ShortNavigation::updateLayLines()
{
    if (debug) qDebug() << "UwShort::updateLayLines(): started";

    // layLines are not calculated with the actual TWA,
    // but the TWA that we will have when heading towards our destiny
    float futureTrueWindAngle = UwMath::getTWA( geoBoatPos, geoDestinyPos, trueWindDirection );
    layLinesAngle = pPolarDiagram->getAngle( windSpeed, futureTrueWindAngle);

    // new paths...
    pLeftPath = new QVector<QPointF>;
    pRightPath = new QVector<QPointF>;

    if ( layLinesAngle != 0 ) {

        // THEN WE ARE NOT REACHING

        if (debug) {
            qDebug() << "UwShort::updateLayLines(): TWD: " << trueWindDirection
                     << " layAng: " << layLinesAngle << " pos: ( " << geoBoatPos.x() << ", "
                     << geoBoatPos.y() << ") ckp: ( " << geoDestinyPos.x() << " ,"
                     << geoDestinyPos.y() << ")";
        }

        QPointF TPleft, TPright;
        // get the turning point without taking care of obstacles
        UwMath::getTurningPoints( TPleft, TPright,
                                  trueWindDirection, layLinesAngle,
                                  geoBoatPos, geoDestinyPos);
        if (debug) {
            qDebug() << "UwShort::updateLayLines(): master turning points: left: ( "
                     << TPleft.x() << " ," << TPleft.y() << ")  right: ( "
                     << TPright.x() << " ," << TPright.y() << ") ";
        }

        // ORDER IS VERY IMPORTANT!!
        // this is our area of interest for obstacles checking
        QPolygonF rhomboid;
        rhomboid << geoBoatPos;
        rhomboid << TPleft;
        rhomboid << geoDestinyPos;
        rhomboid << TPright;

        // QLineF heading( geoBoatPos, geoDestinyPos );
        // Checking of the heading is made when getting the checkpoint now

        // Let's go for obstacles checking...

        if ( checkIntersection( "obstacles_r", rhomboid, rhomboid ) ||
             checkIntersection( "obstacles_l", rhomboid, rhomboid) ) {

            // if we have polygon obstacles in the area
            // OR we have line obstacles in the area

            // populate the right path
            getPath( true, ACCU_OFFSET, MAX_TURNING_POINTS,
                     trueWindDirection, layLinesAngle,
                     geoBoatPos, geoDestinyPos, rhomboid, *pRightPath);
            // populate the left path
            getPath( false, ACCU_OFFSET, MAX_TURNING_POINTS,
                     trueWindDirection, layLinesAngle,
                     geoBoatPos, geoDestinyPos, rhomboid, *pLeftPath);

        } else {

            // if we don't have any obstacle in our area
            // use master turning points for the path

            pRightPath->append( geoBoatPos);
            pRightPath->append( TPright );
            pRightPath->append( geoDestinyPos);

            pLeftPath->append( geoBoatPos);
            pLeftPath->append( TPleft );
            pLeftPath->append( geoDestinyPos);
        }

        // end of checking

    } else {

        // layLines angle = 0
        // THEN WE ARE REACHING

        pRightPath->append( geoBoatPos);
        pRightPath->append( geoDestinyPos);

        pLeftPath->append( geoBoatPos);
        pLeftPath->append( geoDestinyPos);

    }

    if (debug) qDebug() << "UwShort::updateLayLines(): ended ok";


}

void ShortNavigation::processData( const QVector<QPointF> * other_p_geo_route,
                                   QPointF &other_geo_boat_pos, float &other_twd, float &other_wspeed,
                                   PolarDiagram * other_p_pd, QThread::Priority other_priority )
{
    if (debug) qDebug() << "UwShort::processData()";

    this->new_data = true;

    // if the tables are created
    if ( status->obstaclesTablesCreated() ) {

        this->pGeoRoute = other_p_geo_route;
        this->geoBoatPos = other_geo_boat_pos;
        this->trueWindDirection = other_twd;
        this->windSpeed = other_wspeed;
        this->pPolarDiagram = other_p_pd;

        this->start( other_priority);

    } else {

        if (debug) qDebug() << "UwShort::processData(): WARNING: tables are not done, trying to do them...";

        if ( this->createObstaclesTables() ) {

            this->pGeoRoute = other_p_geo_route;
            this->geoBoatPos = other_geo_boat_pos;
            this->trueWindDirection = other_twd;
            this->windSpeed = other_wspeed;
            this->pPolarDiagram = other_p_pd;

            this->start( other_priority);

        } else {

            if (debug) qDebug() << "UwShort::processData(): FATAL: failed to create the tables";

            this->quit();

        }
    }
}

void ShortNavigation::run()
{
    if ( this->new_data) {

        // we have new data to process!
        if (debug) qDebug() << "UwShort::processData(): starting data processing...";

//        if ( !this->hasConn() )
//            this->openConn();

        this->updateCheckPoint();
        this->updateLayLines();

//        this->closeConn();

        this->new_data = false;

        emit finishedProcessing( geoDestinyPos, destinyPos, pLeftPath, pRightPath, layLinesAngle);

        if (debug) qDebug() << "UwShort::processData(): data processed and signal send";

    } else if ( !status->obstaclesTablesCreated() )
        // this is just for the first run
        this->createObstaclesTables();
}

