#include "calculatelaylines.h"


CalculateLaylines::CalculateLaylines(QObject* parent)
    : QObject(parent)
{}

void CalculateLaylines::openPostgreConnection(){

    qDebug() << Q_FUNC_INFO;
    conn = PQconnectdb("hostaddr = '192.168.56.102' port = '5432' dbname = 'chart57' user = 'postgres' password = 'upwind'");
    qDebug() << "Connection ok!";
    if (PQstatus(conn) != CONNECTION_OK){
        qDebug() << "Connection not ok: " << PQerrorMessage(conn);
    }

    this->pPolarDiagram = new PolarDiagram();
}

CalculateLaylines::~CalculateLaylines()
{
    PQfinish(conn);
}

void CalculateLaylines::setPolarDiagram(PolarDiagram *diagram){
    this->pPolarDiagram = diagram;
}

void CalculateLaylines::start(){

    QTimer* timer = new QTimer;
    timer->setInterval(10000);
    connect(timer, SIGNAL(timeout()), this, SLOT(startCalc()));
    timer->start();
}

void CalculateLaylines::setRoutePoints(QPolygonF routePoints){

    this->pathPoints = routePoints;
}

void CalculateLaylines::setStartPoint(QPointF startPoint){

    this->startPoint = startPoint;
}

void CalculateLaylines::receiveData(QVector<QPointF> route, QPointF startpoint){
    this->pathPoints = route;
    this->startPoint = startPoint;
}

void CalculateLaylines::startCalc(){
    qDebug() << "Starting calculation...";
    QVector<QPointF> layLines;
    QVector<QPointF> rightpath;
    QVector<QPointF> leftpath;

    QPolygonF routepoints = this->pathPoints;
    QPointF start = this->startPoint;

    if (routepoints.size() > 0){
        this->openPostgreConnection();

        this->ACCU_OFFSET = 1;
        this->MAX_TURNING_POINTS = 3;

        this->geoBoatPos = start;
        this->pathPoints = routepoints;

        //Start process of calculating laylines. First get the the destination point on long term route (no obstacles between baot and long term route point).
        this->updateCheckPoint();
        //then calculate the laylines that takes the boat to that destination point
        this->updateLayLines();

        rightpath = *pRightPath;
        leftpath = *pLeftPath;
        for(int i = 0; i < rightpath.size(); i++){
            layLines.append(rightpath.at(i));
        }

        for(int i = 0; i < leftpath.size(); i++){
            layLines.append(leftpath.at(i));
        }

        emit emitLaylines(layLines);
    }

}


QString CalculateLaylines::buildWKTPolygon( const QPointF &epoint, const float &offset ) {

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


QPointF CalculateLaylines::getFromWKTPoint( QString wkt_geometry)
{
    // POINT(25.0346075555951 65.2062704150903)

    // remove the end and start
    wkt_geometry.remove( wkt_geometry.size() - 1, 1);
    wkt_geometry.remove( 0, 6);

    return QPointF( wkt_geometry.section(" ",0,0).toDouble(),
                    wkt_geometry.section(" ",1,1).toDouble() );
}


QString CalculateLaylines::buildWKTPolygon( const QPolygonF &rhomboid ) {


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

QString CalculateLaylines::buildWKTPolygon( const QPolygonF &epolygon, const QPointF &ecentroid,
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

QString CalculateLaylines::buildWKTLine( const QPointF &p1, const QPointF &p2 ) {

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

QString CalculateLaylines::buildWKTPoint( const QPointF &p1 ) {


    QString WKTPoint = "'POINT(";
    WKTPoint.append( QString::number( p1.x(), 'f', WKT_P));
    WKTPoint.append( " ");
    WKTPoint.append( QString::number( p1.y(), 'f', WKT_P));
    WKTPoint.append( ")'");

    return WKTPoint;
}

bool CalculateLaylines::checkGeometriesIntersection( const QString &object1, const QString &object2 ) {


    QString sql("SELECT * FROM ( SELECT DISTINCT Intersects( "); //Try to Fix this, Where is Intersect table in database?
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

bool CalculateLaylines::checkIntersection( const QString &layerName, const QString &object, QString shape = QString() ) {


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

    //sql = "SELECT DISTINCT Intersects( wkb_geometry, 'POINT(0.2511030137539 0.6501500010490)'";
    res = PQexec(conn, sql.toAscii() );

    bool intersection = ( PQntuples(res) > 0 );
    PQclear(res);

    return intersection;
}

bool CalculateLaylines::checkIntersection( const QString &layerName, const QPolygonF &triangle, const QPolygonF &rhomboid ) {

    //We'll convert QPointF into String so that we can use it in database queries.
    QString WKTTriangle = buildWKTPolygon( triangle );
    QString WKTPolygon = buildWKTPolygon( rhomboid );

    return checkIntersection( layerName, WKTTriangle, WKTPolygon);

}

bool CalculateLaylines::checkIntersection( const QString &layerName, const QPointF &point ) {

    QString WKTPoint = buildWKTPoint( point );

    return checkIntersection( layerName, WKTPoint);

}

bool CalculateLaylines::publicCheckIntersection( const QString &layerName, const QPointF &point ) {

//Removed 02012012 Rauno
//    if (obstaclesTablesCreated )
//        this->createObstaclesTables();

    QString WKTPoint = buildWKTPoint( point );
    bool result = checkIntersection( layerName, WKTPoint);

    return result;
}

bool CalculateLaylines::checkIntersection( const QString &layerName, const QLineF &heading, const QPolygonF &rhomboid ) {

    QString WKTLine = buildWKTLine( heading.p1(), heading.p2() );
    QString WKTPolygon = buildWKTPolygon( rhomboid );

    return checkIntersection( layerName, WKTLine, WKTPolygon);

}

bool CalculateLaylines::checkIntersection( const QString &layerName, const QLineF &heading ) {

    QString WKTLine = buildWKTLine( heading.p1(), heading.p2() );

    return checkIntersection( layerName, WKTLine);

}

bool CalculateLaylines::checkIntersection( const QLineF &line1, const QLineF &line2 ) {

    QString WKTLine1 = buildWKTLine( line1.p1(), line1.p2() );
    QString WKTLine2 = buildWKTLine( line2.p1(), line2.p2() );

    return checkGeometriesIntersection( WKTLine1, WKTLine2);

}

bool CalculateLaylines::checkOffset( const QPolygonF &last_triangle, const QPolygonF &present_triangle, const QPointF &destiny, const float &offset ) {

    //The distance between original chackpoint and new checkpoint:
    double diff = UwMath::getDistance(last_triangle.at( 2), present_triangle.at( 2));

    //The distance between boat and the checkpoint divided by the distance between boat
    //and new checkpoint:
    double scale = ( UwMath::getDistance(present_triangle.at( 0), destiny) +
                     UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 1)) ) / 2 ;

    //If scale == 0 it means that original checkpoint and new checkpoint are exactly the same point.
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    return diff < criteria;
}

bool CalculateLaylines::checkOffset_OnePointVersion( const QPolygonF &last_triangle, const QPolygonF &present_triangle, const float &offset ) {

    double diff = UwMath::getDistance(last_triangle.at( 2), present_triangle.at( 2));
    double scale = ( UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 2)) +
                     UwMath::getDistance(present_triangle.at( 0), present_triangle.at( 1)) ) / 2 ;
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    return diff < criteria;
}

bool CalculateLaylines::checkOffset( const QLineF &l1, const QLineF &l2, const float &offset ) {

    double diff = UwMath::getDistance(l1.p2(), l2.p2());
    double scale = ( UwMath::getDistance(l1.p1(), l1.p2()) +
                     UwMath::getDistance(l2.p1(), l2.p2()) ) / 2 ;
    double criteria = ( abs(scale) == 0 ) ? offset : offset * scale;

    return diff < criteria;
}

void CalculateLaylines::getPath( const bool &side, const float &offset, const int &max_turns,
                               const double &windAngle, const double &layLinesAngle,
                               const QPointF &boatPos, const QPointF &destinyPos,
                               const QPolygonF &obstacles_shape, QVector<QPointF> &Path )
{
    //Input has to be Geographical data because we check against PostGIS

    QPointF TPleft, TPright;
    bool sideRight = side;
    bool ready, obs_r, obs_l = false;
    int count = 0;
    // Path is returned by reference, clear:
    Path.clear();
    // First point in the path is the origin:
    Path.append( boatPos);
    QPolygonF present_triangle;
    QPolygonF last_triangle;

    // While the last point in the path is not the destiny
    // AND we don't exceed the maximum turning points setting we'll execute following:
    while ( Path.last() != destinyPos && count < max_turns ) {

        // Clear triangle:
        present_triangle.clear();

        // Get the turning points between the last point and dest.
        // For the first run last point in the path is origin:
        UwMath::getTurningPoints( TPleft, TPright,
                                  windAngle, layLinesAngle,
                                  Path.last(), destinyPos);

        // Build a new triangle
        present_triangle << Path.last();
        if (sideRight) present_triangle << TPright;
        else present_triangle << TPleft;
        present_triangle << destinyPos;

        // Check for obstacles in the triangle
        obs_r = checkIntersection( "obstacles_r", present_triangle, obstacles_shape );
        obs_l = checkIntersection( "obstacles_l", present_triangle, obstacles_shape );

        if ( obs_r || obs_l ) {

            // Obstacles found: let's reduce the triangle until they disapear:
            ready = false;

            last_triangle = present_triangle;
            // Reduce triangle to half:
            present_triangle = UwMath::triangleToHalf( present_triangle );

            while ( !ready ) {

                // Check again, put booleans first and checkIntersection won't even be called:
                if ( ( obs_r && checkIntersection( "obstacles_r", present_triangle, obstacles_shape ) ) ||
                     ( obs_l && checkIntersection( "obstacles_l", present_triangle, obstacles_shape ) ) ) {

                    // Still obstacles: reduce again!
                    last_triangle = present_triangle;
                    present_triangle = UwMath::triangleToHalf( present_triangle );

                } else {

                    // No more obstacles:
                    if (  checkOffset( last_triangle, present_triangle, destinyPos, offset ) ) {
                        // The distance is acceptable, finetune done.
                        ready = true;
                    } else {
                        // We are too far, let's increase the triangle a bit:
                        present_triangle = UwMath::avgTriangle( present_triangle, last_triangle);
                    }
                }

            }
        }

        // This triangle has no obstacles inside:
        Path.append( present_triangle.at( 1 ) );
        Path.append( present_triangle.at( 2 ) );
        // Add a turn:
        count++;

        // We continue in the other side:
        sideRight = !sideRight;
    }
}

int CalculateLaylines::getNearestPoint( const QVector<QPointF> &route, const QPointF &boatPos){


    // Input should be in GEOGRAPHICAL format.
    // We receive a route here as a list of points.
    // Let's see at what point of the route we are:
    double distance = std::numeric_limits<double>::max();
    int nearest_point = 0;

    //1. We'll get the shortest distance to boat:
    //Here we get a point where we "join" the route:
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

    qDebug() << "Nearest point: " << route.at(nearest_point) << ", at; " << nearest_point;
    return nearest_point;
}

QPointF CalculateLaylines::getNextPoint( const QVector<QPointF> &route, const QPointF &boatPos, const float &offset) {

    int nearest_point = getNearestPoint(route, boatPos);

    if ( checkIntersection( "obstacles_r", boatPos ) ) {
        // if we are inside an obstacle, don't even try

        return boatPos;
    }

    // we got the nearest point in the route
    // but could be far, let's find out the real
    // nearest point of the route by making a
    // projection towards it...
    // START SEARCH OF PROJECTION FOR FINETUNE


    //2. We'll get projections to each point on the route:
    QPointF projection_point;
    QLineF route_line, projection_line;
    QPointF a, b, c;
    if ( route.size() >= 2 ) {

        // If nearest is the last one:
        if ( route.size() - nearest_point == 1 ) {


            a = UwMath::toConformal( route.at( nearest_point - 1 ) );
            b = UwMath::toConformal( route.at( nearest_point ) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point - 1 ));
            route_line.setP2( route.at( nearest_point ));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

        // If nearest is the first one:
        } else if ( nearest_point == 0 ) {


            a = UwMath::toConformal( route.at( nearest_point) );
            b = UwMath::toConformal( route.at( nearest_point + 1) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point));
            route_line.setP2( route.at( nearest_point + 1));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

        // If nearest is not first or last one:
        } else {


            a = UwMath::toConformal( route.at( nearest_point) );
            b = UwMath::toConformal( route.at( nearest_point + 1) );
            c = UwMath::toConformal( (const QPointF)boatPos );

            projection_point = UwMath::getProjectionPoint( a, b, c);
            UwMath::fromConformal( projection_point);

            route_line.setP1( route.at( nearest_point));
            route_line.setP2( route.at( nearest_point + 1));
            projection_line.setP1( boatPos);
            projection_line.setP2( projection_point);

            // We must check if our position is between nearest and nearest + 1
            // or between nearest - 1 and nearest:
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

        qDebug() << "Route Line" << route_line;
        qDebug() << "Projection line: " << projection_line;
        //It seems that projection point is not on the route:
        qDebug() << "Is projection line on the route? " << UwMath::checkIfBetweenCoordinates(projection_point, route_line.p1(), route_line.p2());
    }


    // Next we'll find the checkpoint:

    //3. We'll check if there are obstacles in the projeted triangle:
    if ( route.size() < 1 ) {


    } else if ( route.size() == 1 ) {

        // Route to process only has one point:
        QLineF heading( boatPos, route.at( nearest_point));

        bool hobs_r = checkIntersection( "obstacles_r", heading );
        bool hobs_l = checkIntersection( "obstacles_l", heading );

        // Finetune checkpoint in the heading:
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
            //We won't return the point between boat and checkpoint as the next checkpoint on long-term route:
            return heading.p2();

        } else {
            //We won't return the point between boat and checkpoint as the next checkpoint on long-term route:
            return heading.p2();
        }

    } else if ( route.size() > 1 ) {

        // Route to process has more than 1 point:
        int i = nearest_point;

        QPolygonF triangle;
        // 1st point:
        triangle << boatPos;

        // 2nd point:
        if ( checkIntersection( route_line, projection_line) ) {
            triangle << projection_point;
        } else {
            triangle << route.at( nearest_point);
        }

        // 3rd point:
        if ( route.at( nearest_point) == route_line.p2() ) {
            triangle << route.at( nearest_point);
            i = nearest_point - 1;
        } else if ( route.at( nearest_point) == route_line.p1() ) {
            triangle << route.at( nearest_point + 1);
        }

        //        // ########################################################################
        //        // SPECIAL CHECK to find obstacles in the heading of the first triangle  ##
        //        // ########################################################################'
//                if ( triangle.at( 0 ) == boatPos ) { //vanity this value is forced at 939
//                    QLineF heading( triangle.at( 0), triangle.at( 1));
//                     QPolygonF last_triangle;

//                    bool hobs_r = checkIntersection( "obstacles_r", heading, triangle );
//                    bool hobs_l = checkIntersection( "obstacles_l", heading, triangle );

//                    // FINETUNE CHECKPOINT IN THE HEADING
//                    if ( hobs_r || hobs_l ) {

//                        bool ready = false;

//                        QLineF last_heading;

//                        while ( !ready ) {

//                            qDebug() <<"splitting segment";

//                            if ( ( hobs_r && checkIntersection( "obstacles_r", heading, triangle) ) ||
//                                 ( hobs_l && checkIntersection( "obstacles_l", heading, triangle)) ) {

////                                last_heading = heading;
////                                heading = UwMath::lineToHalf( heading);
//                                last_triangle = triangle;
//                                triangle = UwMath::triangleToHalf(triangle);

//                            } else {
//                              //  if (  checkOffset( heading, last_heading, offset ) )
//                                if ( checkOffset_OnePointVersion( last_triangle, triangle, offset ))
//                                    ready = true;
//                                else
//                                    triangle = UwMath::avgTriangle_OnePointVersion( triangle, last_triangle);
//                                  //  heading = UwMath::avgLine( heading, last_heading);
//                            }
//                        }
//                       // return heading.p2(); //Ei palauteteta veneen ja checkpointin välistä pistettä seuraavana checkpointina long term routella...
//                        return triangle.at(2);

//                    }
//                }
        // ########################################################################
        // END SPECIAL CHECK                                                     ##
        // ########################################################################

        bool obs_r = checkIntersection( "obstacles_r", triangle, triangle );
        bool obs_l = checkIntersection( "obstacles_l", triangle, triangle );

        qDebug() << "obs_r" << obs_r << "obs_l" << obs_l;

        //Note: This doesn't work correctly. Here the search for the route fails instantly when obstacles
        //are found on the route. What if there is obstacle-free route on the next route interval:

        while ( !obs_r && !obs_l &&  i < (route.size() - 1 )) {

            triangle.clear();

            triangle << boatPos;
            triangle << route.at( i);
            triangle << route.at( i + 1);

            obs_r = checkIntersection( "obstacles_r", triangle, triangle );
            obs_l = checkIntersection( "obstacles_l", triangle, triangle );


            // Finetune checkpoint:

            if ( obs_r || obs_l ) {


                bool ready = false;

                QPolygonF last_triangle;

                while ( !ready ) {

                    if ( obs_r && checkIntersection( "obstacles_r", triangle, triangle) ||
                         obs_l && checkIntersection( "obstacles_l", triangle, triangle) ) {

                        last_triangle = triangle;
                        triangle = UwMath::triangleToHalf_OnePointVersion( triangle);

                    } else {

                        if (  checkOffset_OnePointVersion( last_triangle, triangle, offset ) ){
                            ready = true;
                        }
                        else{
                            triangle = UwMath::avgTriangle_OnePointVersion( triangle, last_triangle);
                        }
                    }
                    //If the points in the line segment on the side of the route there is no space for the boat in
                    //the map anymore and we can quit the search:
                    if(triangle.at(1) == triangle.at(2))
                        ready = true;
                }

            }
            i++;
        }


        return triangle.at( 2);

    }
    // End the process of searching for the checkpoints.

}

void CalculateLaylines::updateCheckPoint()
{

    // Let's find out which is the next point in our route.
    // Find the destiny check point in geographical format:
    geoDestinyPos = this->getNextPoint( this->pathPoints, geoBoatPos, ACCU_OFFSET);

    destinyPos = UwMath::toConformalInverted(geoDestinyPos);

}

void CalculateLaylines::updateLayLines()
{
    QTime timeUpdateLayLines;
    timeUpdateLayLines.start();
    this->pPolarDiagram->populateWithFinngulf36();

    // LayLines are not calculated with the actual TWA,
    // but the TWA that we will have when heading towards our destiny.

    //************HARDCODED VALUE FOR futureTrueWindAngle*************
    this->trueWindDirection = 270.0;
    float futureTrueWindAngle = UwMath::getTWA( geoBoatPos, geoDestinyPos, trueWindDirection );
    //************HARDCODED VALUE FOR windSpeed*************
    windSpeed = 10;

    layLinesAngle = pPolarDiagram->getAngle( windSpeed, futureTrueWindAngle);

    // new paths...
    pLeftPath = new QVector<QPointF>;
    pRightPath = new QVector<QPointF>;

    if ( layLinesAngle != 0 ) {

        // Here we are not reaching:

        QPointF TPleft, TPright;
        // Get the turning point without taking care of obstacles:
        UwMath::getTurningPoints( TPleft, TPright,
                                  trueWindDirection, layLinesAngle,
                                  geoBoatPos, geoDestinyPos);

        // ORDER IS VERY IMPORTANT!!
        // This is our area of interest for obstacles checking
        QPolygonF rhomboid;
        rhomboid << geoBoatPos;
        rhomboid << TPleft;
        rhomboid << geoDestinyPos;
        rhomboid << TPright;

        // QLineF heading( geoBoatPos, geoDestinyPos );
        // Checking of the heading is made when getting the checkpoint here.

        // Next we'll go for obstacles checking:

        if ( checkIntersection( "obstacles_r", rhomboid, rhomboid ) ||
             checkIntersection( "obstacles_l", rhomboid, rhomboid) ) {
            // If we have polygon obstacles in the area
            // OR we have line obstacles in the area:

            // Populate the right path:
            getPath( true, ACCU_OFFSET, MAX_TURNING_POINTS,
                     trueWindDirection, layLinesAngle,
                     geoBoatPos, geoDestinyPos, rhomboid, *pRightPath);
            // Populate the left path:
            getPath( false, ACCU_OFFSET, MAX_TURNING_POINTS,
                     trueWindDirection, layLinesAngle,
                     geoBoatPos, geoDestinyPos, rhomboid, *pLeftPath);


        } else {

            // If we don't have any obstacle in our area
            // use master turning points for the path:
            qDebug() << "UPDATELAYLINES NO OBSTACLES";

            pRightPath->append( geoBoatPos);
            pRightPath->append( TPright );
            pRightPath->append( geoDestinyPos);

            pLeftPath->append( geoBoatPos);
            pLeftPath->append( TPleft );
            pLeftPath->append( geoDestinyPos);
        }

        // End of checking

    } else {

        // LayLines angle = 0
        // Here we are reaching:

        pRightPath->append( geoBoatPos);
        pRightPath->append( geoDestinyPos);

        pLeftPath->append( geoBoatPos);
        pLeftPath->append( geoDestinyPos);

    }

}


