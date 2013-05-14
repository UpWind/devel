/**
 *
 * \file uwmath.h
 * \author Carles Escrig i Royo
 * \date 10-23-09
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
 */

#ifndef UWMATH_H
#define UWMATH_H

#include <cmath>
#include <QPointF>
#include <QLineF>
#include <QPolygonF>
#include <QList>
#include <QVector>
#include <QDebug>

//mean radius of the earth
#define EARTH_RADIUS 6371

class UwMath
{
public:
    UwMath();

    static double mod(const double &x, const double &y);
    static double toRadians(const double &degrees);
    static QPointF toRadians(const QPointF &pointDegrees);
    static double toDegrees(const double &radians);

    static float toPolar(const float &cartesianAngle);
    static float toCartesian(const float &polarAngle);
    static float getDiffPolars(const float &polarAngle1, const float &polarAngle2);

    static double toMercator(const double &latitude);
    static double fromMercator(const double &point);

    // Conformal inverted is special for Qt, it puts the y() value in negative
    // because of the graphical coordinate system
    static void toConformal(QPointF &object);
    static void toConformal(QPolygonF &object);
    static QPointF toConformal(const QPointF &object);
    static QPolygonF toConformal(const QPolygonF &object);
    static void toConformalInverted(QPointF* object);
    static void toConformalInverted(QPolygonF &object);
    static QPointF toConformalInverted(const QPointF &object);
    static QPolygonF toConformalInverted(const QPolygonF &object);
    static QList<QPointF> toConformalInverted(const QList<QPointF> &object);
    static QVector<QPointF> toConformalInverted(const QVector<QPointF> &object);
    static void toConformalInverted(QVector<QPointF> &object);

    static void fromConformal(QPointF &object);
    static void fromConformal(QPolygonF &object);
    static QPointF fromConformal(const QPointF &object);
    static QPolygonF fromConformal(const QPolygonF &object);
    static void fromConformalInverted(QPointF &object);
    static void fromConformalInverted(QPolygonF &object);
    static QPointF fromConformalInverted(const QPointF &object);
    static QPolygonF fromConformalInverted(const QPolygonF &object);
    static QList<QPointF> fromConformalInverted(const QList<QPointF> &object);
    static QVector<QPointF> fromConformalInverted(const QVector<QPointF> &object);

    static double getCartesianAngleWithDestiny(const double &lat1, const double &lon1,
                                                const double &lat2, const double &lon2);
    static double getCartesianAngleWithDestiny(const QPointF &origin, const QPointF &destiny);
    static double getPolarAngleWithDestiny(const double &lat1, const double &lon1,
                                            const double &lat2, const double &lon2);
    static double getPolarAngleWithDestiny(const QPointF &origin, const QPointF &destiny);
    static double getDistance(const double &lat1, const double &lon1,
                               const double &lat2, const double &lon2, double earth_rad = EARTH_RADIUS);
    static double getDistance(const QPointF &origin, const QPointF &destiny, double earth_rad = EARTH_RADIUS);
    static double getTWA(const double &lat1 , const double &lon1,
                         const double &lat2, const double &lon2, const float &wAngle);
    static double getTWA(const QPointF &origin, const QPointF &destiny, const float &wAngle);

    static void getTurningPoints(double &leftTPlat, double &leftTPlon, double &rightTPlat, double &rightTPlon,
                                 const float &wAngle, const float &layLinesAngle,
                                 const double &lon1, const double &lat1, const double &lon2, const double &lat2);
    static void getTurningPoints(QPointF &leftTP, QPointF &rightTP,
                                 const float &wAngle, const float &layLinesAngle,
                                 const QPointF &origin, const QPointF &destiny);
    static void getMiddlePoint(double &lat, double &lon,
                               const double &lat1, const double &lon1,
                               const double &lat2, const double &lon2);

    static QPointF getMiddlePoint(const QPointF &origin, const QPointF &destiny);
    static QPolygonF triangleToHalf(const QPolygonF &original);
    static QPolygonF avgTriangle(const QPolygonF &tr1, const QPolygonF &tr2);
    static QPolygonF triangleToHalf_OnePointVersion(const QPolygonF &original);
    static QPolygonF avgTriangle_OnePointVersion(const QPolygonF &tr1, const QPolygonF &tr2);
    static QLineF lineToHalf(const QLineF &l1);
    static QLineF avgLine(const QLineF &l1, const QLineF &l2);

    static void getAnotherPoint(double &Plat, double &Plon,
                                const float &wAngle,
                                const double &lon1, const double &lat1);
    static void getAnotherPoint(QPointF &back, const float &wAngle, const QPointF &point);
    static QPointF getProjectionPoint(const QPointF &a, const QPointF &b, const QPointF &c);
    static bool checkIfBetweenCoordinates(QPointF a, QPointF b, QPointF c);
};

#endif // UWMATH_H
