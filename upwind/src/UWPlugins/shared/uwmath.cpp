/**
 * \file uwmath.cpp
 * \author Carles Escrig i Royo
 * \date 10-23-09 (stable)
 *
 * Copyright (C) 2010 by University of Oulu (Finland)
 *
 * This file is part of UpWind Project.
 *
 * UwMath library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UwMath library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UwMath library.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include "uwmath.h"

/** The double type is used for data, if more precision is needed
 * also the constants from C math library should be changed adding
 * a 'l' character. For example: M_PI -> M_PIl
 */

UwMath::UwMath(){}

double UwMath::mod( const double &x, const double &y){
    return x - y * floor(x / y);
}

/** Translate from degrees to radians
  * @param radians
  * @return degrees
  */
double UwMath::toRadians(const double &degrees){
    return degrees * M_PI / 180.0;
}

/** Translate from radians to degrees
  * @param radians
  * @return degrees
  */
double UwMath::toDegrees(const double &radians){
    return radians * 180.0 / M_PI;
}

/** Translate an angle in degrees from Cartesian Format ( 0° is 3 o'clock,
  * movement clockwise is negative, movement counter clockwise is positive )
  * to Polar Format ( ranges from 0° to 360° where 0° is 12 o'clock).
  * @param angle in degrees (Cartesian Format)
  * @return angle in degrees (Polar Format)
  */
float UwMath::toPolar(const float &cartesianAngle){
    float polarAngle;

    if(cartesianAngle < 0)
        polarAngle =  90 + (cartesianAngle * (-1));
    else if (cartesianAngle > 90)
        polarAngle = (180 - cartesianAngle) + 270;
    else
        polarAngle = 90 - cartesianAngle;

    return polarAngle;
}

/** Translate an angle in degrees from Polar Format (ranges from 0° to 360°
  * where 0° is 12 o'clock) to Cartesian Format (0° is 3 o'clock,
  * movement clockwise is negative, movement counter clockwise is positive)
  * @param angle in degrees (Cartesian Format)
  * @return angle in degrees (Polar Format)
  */
float UwMath::toCartesian(const float &polarAngle){
    float cartAngle;

    if(polarAngle <= 90)
        cartAngle = 90 - polarAngle;
    else if(polarAngle > 90 && polarAngle <= 180)
        cartAngle = (polarAngle - 90) * (-1);
    else if(polarAngle > 180 && polarAngle <= 270)
        cartAngle = (polarAngle - 90) * (-1);
    else
        cartAngle = 450 - polarAngle;
    return cartAngle;
}

/** Calculates the difference between two angles in degrees (Polar Format)
  * always gets the interior part (the smaller).
  * @param angle in degrees (Polar Format)
  * @param angle in degrees (Polar Format)
  * @return difference in degrees
  */
float UwMath::getDiffPolars(const float &polarAngle1, const float &polarAngle2){
    float cart1 = UwMath::toCartesian(polarAngle1);
    float cart2 = UwMath::toCartesian(polarAngle2);
    float diff;

    if(cart1 >= 0 && cart2 >= 0)
        diff = ( cart1 >= cart2 ) ? cart1 - cart2 : cart2 - cart1;
    else if ( cart1 <= 0 && cart2 <= 0)
        diff = (cart1 < cart2) ? fabs(cart1 - cart2) : fabs(cart2 - cart1);
    else {
        diff = (polarAngle1 > polarAngle2) ? polarAngle1 - polarAngle2 : polarAngle2 - polarAngle1;
        if(diff > 180.0)
            diff = 180.0 - UwMath::mod(diff, 180.0);
    }
    return diff;
}

/** Calculate the equivalent conformal point of latitude (mercator)
  * from a given latitude presented in DD Decimal Degrees (-123.5000°) format
  * @param latitude of geographical coordenate
  * @return returns the latitude as a conformal point in radians
  */
double UwMath::toMercator(const double &latitude){
    return log(tan((UwMath::toRadians(latitude) / 2.0) + M_PI_4));
}

/** Recalculate the equivalent in degrees of conformal point of latitude
  * @param point of latitude in mercator projection using radians
  * @return returns the Latitude as Decimal Degree
  */
double UwMath::fromMercator(const double &point){
    return UwMath::toDegrees(2 * (atan(pow(M_E, point)) - M_PI_4));
}

/** Converts the point from degrees to a conformal point in
  * Mercator projection
  * @param point in longitude, latitude given in degrees (DD)
  * @return conformal point in radians
  */
void UwMath::toConformal(QPointF &object){
    object.setX(UwMath::toRadians(object.x()));
    object.setY(UwMath::toMercator(object.y()));
}

/** Converts a list of points from degrees to a conformal
  * point in Mercator projection
  * @param points in longitude, latitude given in degrees (DD)
  * @return conformal points in radians
  */
void UwMath::toConformal(QPolygonF &object){
    QPointF * data = object.data();

    for(int i = 0; i < object.size(); i++){
        data[i].setX(UwMath::toRadians(object.at(i).x()));
        data[i].setY(UwMath::toMercator(object.at(i).y()));
    }
}

/** Returns the conversion of a point from degrees to a
  * conformal point in Mercator projection.
  * @param point in longitude, latitude given in degrees (DD)
  * @return conformal point in radians
  */
QPointF UwMath::toConformal(const QPointF &eobject){
    QPointF object(eobject);

    UwMath::toConformal(object);
    return object;
}

/** Returns the conversion of a list of points from degrees to
  * conformal points in Mercator projection.
  * @param points in longitude, latitude given in degrees (DD)
  * @return conformal points in radians
  */
QPolygonF UwMath::toConformal(const QPolygonF &eobject){
    QPolygonF object(eobject);

    UwMath::toConformal(object);
    return object;
}

void UwMath::toConformalInverted(QPointF &object){
    object.setX(UwMath::toRadians(object.x()));
    object.setY(UwMath::toMercator(object.y()) * (-1));
}

void UwMath::toConformalInverted(QPolygonF &object){
    QPointF * data = object.data();

    for(int i = 0; i < object.size(); i++){
        data[i].setX(UwMath::toRadians(object.at(i).x()));
        data[i].setY(UwMath::toMercator(object.at(i).y() * (-1)));
    }
}

QPointF UwMath::toConformalInverted(const QPointF &eobject){
    QPointF object(eobject);

    UwMath::toConformalInverted(object);
    return object;
}

QPolygonF UwMath::toConformalInverted(const QPolygonF &eobject){
    QPolygonF object(eobject);

    UwMath::toConformalInverted(object);
    return object;
}

QList<QPointF> UwMath::toConformalInverted(const QList<QPointF> &object){
    QList<QPointF> temp;

    for(int i = 0; i < object.size(); i++)
        temp.append(UwMath::toConformalInverted(object.at(i)));
    return temp;
}

QVector<QPointF> UwMath::toConformalInverted(const QVector<QPointF> &object){
    QVector<QPointF> temp;

    for(int i = 0; i < object.size(); i++)
        temp.append(UwMath::toConformalInverted(object.at(i)));
    return temp;
}

void UwMath::toConformalInverted(QVector<QPointF> &object){
    QPointF * data = object.data();

    for(int i = 0; i < object.size(); i++)
        data[i] = UwMath::toConformalInverted(object.at(i));
}

void UwMath::fromConformal( QPointF &object){
    object.setX(UwMath::toDegrees(object.x()));
    object.setY(UwMath::fromMercator(object.y()));
}

void UwMath::fromConformal(QPolygonF &object){
    QPointF * data = object.data();

    for(int i = 0; i < object.size(); i++){
        data[i].setX(UwMath::toDegrees(object.at(i).x()));
        data[i].setY(UwMath::fromMercator(object.at(i).y()));
    }
}

QPointF UwMath::fromConformal(const QPointF &eobject){
    QPointF object(eobject);

    UwMath::fromConformal(object);
    return object;
}

QPolygonF UwMath::fromConformal(const QPolygonF &eobject){
    QPolygonF object(eobject);

    UwMath::fromConformal(object);
    return object;
}

void UwMath::fromConformalInverted(QPointF &object){
    object.setX(UwMath::toDegrees(object.x()));
    object.setY(UwMath::fromMercator((-1) * object.y()));
}

void UwMath::fromConformalInverted(QPolygonF &object){
    QPointF * data = object.data();

    for(int i = 0; i < object.size(); i++){
        data[i].setX(UwMath::toDegrees(object.at(i).x()));
        data[i].setY(UwMath::fromMercator(object.at(i).y() * (-1)));
    }
}

QPointF UwMath::fromConformalInverted(const QPointF &eobject){
    QPointF object(eobject);

    UwMath::fromConformalInverted(object);
    return object;
}

QPolygonF UwMath::fromConformalInverted(const QPolygonF &eobject) {
    QPolygonF object(eobject);

    UwMath::fromConformalInverted(object);
    return object;
}

QList<QPointF> UwMath::fromConformalInverted(const QList<QPointF> &object){
    QList<QPointF> temp;

    for(int i = 0; i < object.size(); i++)
        temp.append(UwMath::fromConformalInverted(object.at(i)));
    return temp;
}

QVector<QPointF> UwMath::fromConformalInverted(const QVector<QPointF> &object){
    QVector<QPointF> temp;

    for(int i = 0; i < object.size(); i++)
        temp.append(UwMath::fromConformalInverted(object.at(i)));
    return temp;
}

/** Calculate the angle between two points of coordinates in
  * DD Decimal Degrees (49.5000°,-123.5000°) format
  * @param Latitude of the point used as origin in decimal degrees
  * @param Longitude of the point used as origin in decimal degrees
  * @param Latitude of the point used as destiny in decimal degrees
  * @param Longitude of the point used as destiny in decimal degrees
  * @return the angle in degrees (Cartesian Format)
  */
double UwMath::getCartesianAngleWithDestiny(const double &elon1, const double &elat1,
                                            const double &elon2, const double &elat2)
{
    double lat1 = UwMath::toMercator(elat1);
    double lat2 = UwMath::toMercator(elat2);
    double lon1 = UwMath::toRadians(elon1);
    double lon2 = UwMath::toRadians(elon2);

    // method from: http://mysite.du.edu/~jcalvert/math/mercator.htm
    // everything should be conformal now so.... go on!
    double latDist = fabs(lat2 - lat1);

    // difference between both conformal points of longitude (any hemisphere)
    double lonDist = fabs(lon1 - lon2);

    // angle between both coordenate points given by trigonometry
    double angle = UwMath::toDegrees(atan(latDist / lonDist));

    // but we miss the direction since those are ABS values
    // supplementary angle because we go towards west
    if(((lon1 > 0 && lon2 > 0) && (lon1 > lon2)) ||
            ((lon1 < 0 && lon2 < 0) && (lon1 > lon2)) ||
            (((lon1 < 0 && lon2 > 0) || (lon1 > 0 && lon2 < 0)) && (lon1 > lon2)))
        angle = 180 - angle;

    // negative angle because we go towards south
    if(((lat1 > 0 && lat2 > 0) && (lat1 > lat2)) ||
            ((lat1 < 0 && lat2 < 0) && (lat1 > lat2)) ||
            (((lat1 < 0 && lat2 > 0) || (lat1 > 0 && lat2 < 0)) && (lat1 > lat2)))
        angle = angle * (-1);
    return angle;
}

/** This is an overloaded function for easy of use with Qt toolkit.
  * Calculate the angle between two points of coordinates in
  * DD Decimal Degrees (49.5000°,-123.5000°) format
  * @param point of origin
  * @param point of destiny
  * @return the angle in degrees (Cartesian Format)
  */
double UwMath::getCartesianAngleWithDestiny(const QPointF &origin, const QPointF &destiny){
    return UwMath::getCartesianAngleWithDestiny(origin.x(), origin.y(), destiny.x(), destiny.y());
}

/** Calculate the angle between two points of coordinates in
  * DD Decimal Degrees (49.5000°,-123.5000°) format.
  * @param Latitude of the point used as origin in decimal degrees
  * @param Longitude of the point used as origin in decimal degrees
  * @param Latitude of the point used as destiny in decimal degrees
  * @param Longitude of the point used as destiny in decimal degrees
  * @return the angle in degrees (Polar Format)
  */
double UwMath::getPolarAngleWithDestiny(const double &lon1, const double &lat1,
                                        const double &lon2, const double &lat2)
{
    return UwMath::toPolar(UwMath::getCartesianAngleWithDestiny(lon1, lat1, lon2, lat2));
}

/** This is an overloaded function for easy of use with Qt toolkit.
  * Calculate the angle between two points of coordinates in
  * DD Decimal Degrees (49.5000°,-123.5000°) format
  * @param point of origin
  * @param point of destiny
  * @return the angle in degrees (Polar Format)
  */
double UwMath::getPolarAngleWithDestiny(const QPointF &origin, const QPointF &destiny){
    return UwMath::getPolarAngleWithDestiny(origin.x(), origin.y(), destiny.x(), destiny.y());
}

/** Calculate the distance between two points of coordinates in DD Decimal Degrees
  * (49.5000°,-123.5000°) format. The output will depend on the Earth Radius given.
  * By default it will use 6378 km. But you can use also 6371 km, 3959 mi, 11799.09 nm
  * 1 Nautical Mile is 1.85200 km or 1.15078 mi
  * @param Latitude of the point used as origin in decimal degrees
  * @param Longitude of the point used as origin in decimal degrees
  * @param Latitude of the point used as destiny in decimal degrees
  * @param Longitude of the point used as destiny in decimal degrees
  * @param Earth radius, by default 6378 km
  * @return the distance in units (depends on input, by default km)
  */
double UwMath::getDistance(const double &lon1, const double &lat1,
                           const double &lon2, const double &lat2, double earth_rad)
{
    // method from: http://mysite.du.edu/~jcalvert/math/mercator.htm
    // get both latitudes as conformal
    double orLatMerc = UwMath::toMercator(lat1);
    double deLatMerc = UwMath::toMercator(lat2);

    // difference between both conformal points of latitude (any hemisphere)
    double latDist = fabs(deLatMerc - orLatMerc);

    // difference between both conformal points of longitude (any hemisphere)
    double lonDist = UwMath::toRadians(fabs(lon1 - lon2));

    // distance between both coordenate points given by pythagoras' theorem
    double hypotenuse = sqrt(pow(latDist, 2) + pow(lonDist, 2));

    // the average between latitudes will be correct since they are conformal
    double avgLatDeg = UwMath::fromMercator((deLatMerc + orLatMerc) / 2);

    // calculate the scale of measure for the given latitude
    double scale = earth_rad * cos(UwMath::toRadians(avgLatDeg));

    // The scale of measure times the conformal distance is equal to
    // the distance in earth_rad units.
    double distance = scale * hypotenuse;

    return distance;
}

/** This is an overloaded function for easy of use with Qt toolkit.
  * Calculate the distance between two points of coordinates in DD Decimal Degrees
  * (49.5000°,-123.5000°) format. The output will depend on the Earth Radius given.
  * By default it will use 6378 km. But you can use also 6371 km, 3959 mi, 11799.09 nm
  * 1 Nautical Mile is 1.85200 km or 1.15078 mi
  * @param point of origin
  * @param point of destiny
  * @param Earth radius, by default 6378 km
  * @return the distance in units (depends on input, by default km)
  */
double UwMath::getDistance(const QPointF &origin, const QPointF &destiny, double earth_rad){
    return UwMath::getDistance( origin.x(), origin.y(), destiny.x(), destiny.y(), earth_rad);
}

/** Calculate the True Wind Angle of the course and returns it in Polar Format
  * @param Latitude of the point used as origin in decimal degrees
  * @param Longitude of the point used as origin in decimal degrees
  * @param Latitude of the point used as destiny in decimal degrees
  * @param Longitude of the point used as destiny in decimal degrees
  * @param True Wind Direction (in polar format)
  * @return True Wind Angle
*/
double UwMath::getTWA(const double &lon1, const double &lat1,
                      const double &lon2, const double &lat2, const float &wAngle)
{
    // the difference between the heading -or future heading- and the absolute
    // True Wind Direction is the True Wind Angle for that heading
    // By default we use boatPos and destinyPos from the Boat object
    float angleWD = getPolarAngleWithDestiny(lon1, lat1, lon2, lat2);

    return UwMath::mod(getDiffPolars(angleWD, wAngle), 180.0);
}

/** This is an overloaded function for easy of use with Qt toolkit.
  * Calculate the True Wind Angle of the course and returns it in Polar Format
  * @param point of origin
  * @param point of destiny
  * @param True Wind Direction (in polar format)
  * @return True Wind Angle
  */
double UwMath::getTWA(const QPointF &origin, const QPointF &destiny, const float &wAngle){
    // overloaded fot Qt
    return UwMath::getTWA(origin.x(), origin.y(), destiny.x(), destiny.y(), wAngle);
}

void UwMath::getTurningPoints(
        double &leftTPlon, double & leftTPlat, double & rightTPlon, double & rightTPlat,
        const float &wAngle, const float &layLinesAngle,
        const double &lon1, const double &lat1, const double &lon2, const double &lat2)
{

    double y1 = UwMath::toMercator(lat1);
    double y2 = UwMath::toMercator(lat2);
    double x1 = UwMath::toRadians(lon1);
    double x2 = UwMath::toRadians(lon2);

    // the input data is already converted to conformal
    double cartesianAngleWithDestiny = UwMath::getCartesianAngleWithDestiny(x1, y1, x2, y2);

    // navigation rhomboid interior angle from origin or destiny points
    double fullInAngle = (layLinesAngle > 90) ?
                (180.0 - layLinesAngle) * 2 : layLinesAngle * 2;

    // the angle of the lay line left of the heading, in polar format
    double layLineLeftAngle = (layLinesAngle > 90) ?
                UwMath::mod(wAngle + layLinesAngle, 360.0) :
                UwMath::mod(wAngle - layLinesAngle, 360.0);

    // triangle in the left half of the rhomboid
    double alpha = UwMath::getDiffPolars(layLineLeftAngle,
                                         UwMath::toPolar(cartesianAngleWithDestiny));
    /* double beta = fullInAngle - alpha;
       double gamma = 180.0 - beta - alpha;
       with the sinus rule, the distances to the turning point can be found, for instance
       distance_to_left_turning_point = sin(beta) * distance_with_destiny / sin(gamma)
       Input the angles in Radians, this is mercator!! (conversion needed later)
    */

    // AngleWithXAxis is always in cartesian format, as if ready to draw
    double alphaAngleWithXAxis = UwMath::toCartesian(layLineLeftAngle);
    double betaAngleWithXAxis = alphaAngleWithXAxis - fullInAngle;

    // ecuations of the lines: y = b + mx (where m is the slope)

    // vertical lines have infinite slope, use 0 instead.
    double m1 = (fabs(alphaAngleWithXAxis) != 90.0) ? tan(UwMath::toRadians(alphaAngleWithXAxis)) : 0;
    double m2 = (fabs(betaAngleWithXAxis) != 90.0) ? tan(UwMath::toRadians(betaAngleWithXAxis)) : 0;

    double b1 = y1 - (m1 * x1);
    double b2 = y2 - (m2 * x2);

    // It is assumed that lines are not parallel
    double xi = -( b1 - b2) / (m1 -m2);
    double yi = b1 + m1 * xi;

    leftTPlon = xi;
    leftTPlat = yi;

    // for the triangle in the right of the heading exchange slopes
    // because the sides of the rhomboid are parallel with the oposite
    double aux = m1;
    m1 = m2 ;
    m2 = aux;

    b1 = y1 - (m1 * x1);
    b2 = y2 - (m2 * x2);

    // It is assumed that lines are not parallel
    xi = -(b1 - b2) / (m1 - m2);
    yi = b1 + m1 * xi;

    rightTPlon = xi;
    rightTPlat = yi;

    // Points of turning are now in place by reference
    // transform them back to Geographical
    leftTPlon = UwMath::toDegrees(leftTPlon);
    leftTPlat = UwMath::fromMercator(leftTPlat);
    rightTPlon = UwMath::toDegrees(rightTPlon);
    rightTPlat = UwMath::fromMercator(rightTPlat);
}

void UwMath::getTurningPoints(QPointF &leftTP, QPointF &rightTP,
                              const float &wAngle, const float &layLinesAngle,
                              const QPointF &origin, const QPointF &destiny)
{
    // overloaded for Qt
    double leftTPlon;
    double leftTPlat;
    double rightTPlon;
    double rightTPlat;

    UwMath::getTurningPoints(leftTPlon, leftTPlat, rightTPlon, rightTPlat,
                             wAngle, layLinesAngle,
                             origin.x(), origin.y(), destiny.x(), destiny.y());

    leftTP.setY(leftTPlat);
    leftTP.setX(leftTPlon);
    rightTP.setY(rightTPlat);
    rightTP.setX(rightTPlon);
}

/* testing */

void UwMath::getMiddlePoint(
        double &lon, double &lat,
        const double &lon1, const double &lat1, const double &lon2, const double &lat2)
{
    double y1 = UwMath::toMercator(lat1);
    double y2 = UwMath::toMercator(lat2);
    double x1 = UwMath::toRadians(lon1);
    double x2 = UwMath::toRadians(lon2);
    // difference between both conformal points of latitude (any hemisphere)
    double latDist = fabs(y2 - y1);
    // difference between both conformal points of longitude (any hemisphere)
    double lonDist = fabs(x1 - x2);
    // put the middle point in geographical format
    lat = (y1 < y2) ?
                UwMath::fromMercator(y1 + latDist / 2) :
                UwMath::fromMercator(y2 + latDist / 2);
    lon = (x1 < x2) ?
                UwMath::toDegrees(x1 + lonDist / 2) :
                UwMath::toDegrees(x2 + lonDist / 2);
}

QPointF UwMath::getMiddlePoint(const QPointF &origin, const QPointF &destiny){
    QPointF middlepoint;
    double lon, lat;

    UwMath::getMiddlePoint(lon, lat,
                           origin.x(), origin.y(),
                           destiny.x(), destiny.y());
    middlepoint.setX(lon);
    middlepoint.setY(lat);

    return middlepoint;
}

QPolygonF UwMath::triangleToHalf(const QPolygonF &original){
    QPolygonF half;

    half << original.at(0);
    half << UwMath::getMiddlePoint(original.at(0), original.at(1));
    half << UwMath::getMiddlePoint(original.at(0), original.at(2));
    return half;
}

QPolygonF UwMath::avgTriangle(const QPolygonF &tr1, const QPolygonF &tr2){
    QPolygonF avg;

    avg << tr1.at(0);
    avg << UwMath::getMiddlePoint(tr1.at(1), tr2.at(1));
    avg << UwMath::getMiddlePoint(tr1.at(2), tr2.at(2));
    return avg;
}

QPolygonF UwMath::triangleToHalf_OnePointVersion(const QPolygonF &original){
    QPolygonF half;

    half << original.at(0);
    half << original.at(1);
    half << UwMath::getMiddlePoint(original.at(1), original.at(2));
    return half;
}

QPolygonF UwMath::avgTriangle_OnePointVersion(const QPolygonF &tr1, const QPolygonF &tr2){
    QPolygonF avg;

    avg << tr1.at(0);
    avg << tr1.at(1);
    avg << UwMath::getMiddlePoint(tr1.at(2), tr2.at(2));
    return avg;
}

QLineF UwMath::lineToHalf(const QLineF &original){
    QLineF half(original.p1(), UwMath::getMiddlePoint(original.p1(), original.p2()));
    return half;
}

QLineF UwMath::avgLine(const QLineF &l1, const QLineF &l2){
    QLineF avgl(l1.p1(), UwMath::getMiddlePoint(l1.p2(), l2.p2()));

    return avgl;
}

void UwMath::getAnotherPoint(double &Plon, double &Plat,
                             const float &wAngle,
                             const double &lon1, const double &lat1)
{

    double y1 = UwMath::toMercator(lat1);
    double x1 = UwMath::toRadians(lon1);
    double angle = UwMath::toCartesian( wAngle);

    if(angle < 0)
        angle = 180 + angle;

    // ecuations of the lines: y = b + mx (where m is the slope)

    // vertical lines have infinite slope, use 0 instead.
    double m1 = (fabs(angle) != 90.0) ? tan(UwMath::toRadians(angle)) : 0;

    double b1 = y1 - (m1 * x1);

    Plat = x1 + 0.1;
    Plon = b1 + m1 * Plat;

    // reconvert to Geographical coordenates and put on place
    Plon = UwMath::toDegrees(Plon);
    Plat = UwMath::fromMercator(Plat);
}

void UwMath::getAnotherPoint(QPointF &back, const float &wAngle, const QPointF &point){
    double lon, lat;
    UwMath::getAnotherPoint(lon, lat,
                            wAngle,
                            point.x(), point.y());
    back.setX(lon);
    back.setY(lat);
}

/**
  * @param conformal points in radians
  */
QPointF UwMath::getProjectionPoint(const QPointF &a, const QPointF &b, const QPointF &c){
    QPointF projection_point;

    double r = ((a.y() - c.y()) * (a.y() - b.y()) - (a.x() - c.x()) * (b.x() - a.x())) /
            (pow(sqrt(pow((b.x() - a.x()), 2) + pow((b.y() - a.y()), 2)), 2));

    projection_point.setX(a.x() + r * (b.x() - a.x()));
    projection_point.setY(a.y() + r * (b.y() - a.y()));

    return projection_point;
}

bool UwMath::checkIfBetweenCoordinates(QPointF a, QPointF b, QPointF c){


    if ((b.y()-a.y())/(b.x()-a.x())==(c.y()-b.y())/(c.x()-b.x()))
        return true;
    else
        return false;

}
