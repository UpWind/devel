/**
 * Functions for general tasks.
 * @date 2009-12-26
 */
#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QPointF>
#include <QPolygonF>
#include <QVector>
#include <cmath>
#include <QDebug>

/** Clear zeroes from the end of QString if it looks like a decimal number
 * (contains '.').
 */
QString clearZeroes(QString str);

/** Base function for the point conversion to string.
  */
QString toDMS(const QPointF &point, bool min, bool sec);

/** Converts a point into a string in DMS format.
  * Degrees:Minutes:Seconds (49°30'00"N, 123°30'00"W)
  */
QString toDMS(const QPointF &point);

/** Converts a point into a string in DM format.
  * DM Degrees:Decimal Minutes (49°30.000'N, 123°30.000'W)
  */
QString toDM(const QPointF &point);

/** Converts a point into a string in DD format.
  * DD Decimal Degrees (49.5000°N,123.5000°W)
  */
QString toDD(const QPointF &point);

QPolygonF getFromWKTLinestring(QString wkt_geometry);
QString setToWKTLinestring(const QPolygonF &line);
QVector<QPolygonF> getFromWKTPolygon(QString wkt_geometry);
QString setToWKTPolygon(const QVector<QPolygonF> &poly);

#endif // UTILS_H
