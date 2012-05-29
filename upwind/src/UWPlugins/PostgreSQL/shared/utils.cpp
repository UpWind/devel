#include "utils.h"
#define WKT_P 13

/** Removes trailing zeroes from a number string
  */
QString clearZeroes(QString str){
    if(str.contains('.'))
        str.chop(str.size() - str.lastIndexOf("."));
    return str;
}

/** Transform a coordinate from degrees
  * to Degrees (Minutes (Seconds)).
  */
QString toDMS(const QPointF &point, bool min, bool sec){
    float longi_deg = point.x(), latit_deg = point.y();
    float longi_min, latit_min;
    float longi_sec, latit_sec;
    int longi_cardinal = (longi_deg >= 0) ? 'E' : 'W';
    int latit_cardinal = (latit_deg >= 0) ? 'N' : 'S';
    QString dms;

    if(min){
        longi_deg = floor(longi_deg);
        longi_min = (point.x() - longi_deg) * 60.0;

        latit_deg = floor(latit_deg);
        latit_min = (point.y() - latit_deg) * 60.0;
        if(sec){
            longi_min = floor(longi_min);
            longi_sec = ((point.x() - longi_deg) * 60.0 - longi_min) * 60;

            latit_min = floor(latit_min);
            latit_sec = ((point.y() - latit_deg) * 60.0 - latit_min) * 60;
        }
    }

    dms.append(QString::number(longi_deg, 'f', (min && sec) ? 0 : 4));
    dms.append("°");
    if(min){
        dms.append(QString::number(longi_min, 'f', (sec) ? 0 : 3));
        dms.append("'");
        if(sec){
            dms.append(QString::number(longi_sec, 'f', 2));
            dms.append("\"");
        }
    }
    dms.append(longi_cardinal);
    dms.append(", ");

    dms.append(QString::number(latit_deg, 'f', (!min && !sec) ? 0 : 4));
    dms.append("°");
    if(min){
        dms.append(QString::number(latit_min, 'f', (sec) ? 0 : 3));
        dms.append("'");
        if(sec){
            dms.append(QString::number(latit_sec, 'f', 2));
            dms.append("\"");
        }
    }
    dms.append(latit_cardinal);

    return dms;
}

QString toDMS(const QPointF &point){
    return toDMS(point, true, true);
}

QString toDM(const QPointF &point){
    return toDMS(point, true, false);
}

QString toDD(const QPointF &point){
    return toDMS(point, false, false);
}


QPolygonF getFromWKTLinestring( QString wkt_geometry){
    // LINESTRING(25.0347828111012 65.2061203222919,25.0346594449418 65.2061509400179,...,25.0347828111012 65.2061203222919)

    // remove the end and start
    wkt_geometry.remove(wkt_geometry.size() - 1, 1);
    wkt_geometry.remove(0, 11);

    // explode the string by "," and get a list
    QStringList pointsList = wkt_geometry.split(",");
    QPolygonF polygon;

    // explode each string of the list by " "
    for(int i = 0; i < pointsList.size(); i++){
        polygon << QPointF(pointsList[i].section(" ", 0, 0).toDouble(),
                           pointsList[i].section(" ", 1, 1).toDouble());
    }
    return polygon;
}

QString setToWKTLinestring( const QPolygonF &line){
    // LINESTRING(25.0347828111012 65.2061203222919,25.0346594449418 65.2061509400179,...,25.0347828111012 65.2061203222919)

    int i;
    QString WKTLine("'LINESTRING(");

    for(i = 0; i < line.size() - 1; i++){
        WKTLine.append(QString::number(line.at(i).x(), 'f', WKT_P));
        WKTLine.append(" ");
        WKTLine.append(QString::number(line.at(i).y(), 'f', WKT_P));
        WKTLine.append(",");

    }

    WKTLine.append(QString::number( line.at(i).x(), 'f', WKT_P));
    WKTLine.append(" ");
    WKTLine.append(QString::number( line.at(i).y(), 'f', WKT_P));
    WKTLine.append(")'");

    return WKTLine;
}

QVector<QPolygonF> getFromWKTPolygon(QString wkt_geometry){
    // POLYGON((25.0347828111012 65.2061203222919,25.0346594449418 65.2061509400179),(...),(25.0347828111012 65.2061203222919))

    QVector<QPolygonF> finalPolygon;

    // remove the end and start
    wkt_geometry.remove(wkt_geometry.size() - 2, 2);
    wkt_geometry.remove(0, 9);

    // explode the string by "," and get a list
    QStringList ringList = wkt_geometry.split("),(");

    // explode each string of the list by " "
    for(int i = 0; i < ringList.size(); i++){
        // explode the string by "," and get a list
        QStringList ringPointsList = ringList.at(i).split(",");

        QPolygonF polygon;

        // explode each string of the list by " "
        for(int j = 0; j < ringPointsList.size(); j++){
            polygon << QPointF(ringPointsList[j].section(" ", 0, 0).toDouble(),
                               ringPointsList[j].section(" ", 1, 1).toDouble());
        }
        finalPolygon.append(polygon);
    }
    return finalPolygon;
}

QString setToWKTPolygon(const QVector<QPolygonF> &poly){
    // POLYGON((25.0347828111012 65.2061203222919,25.0346594449418 65.2061509400179),(...),(25.0347828111012 65.2061203222919))

    QString WKTPoly("'POLYGON(");

    for(int i = 0; i < poly.size(); i++){
        WKTPoly.append("(");
        int j;
        for(j = 0; j < poly.at(i).size() - 1; j++){
            WKTPoly.append(QString::number(poly.at(i).at(j).x(), 'f', WKT_P));
            WKTPoly.append(" ");
            WKTPoly.append(QString::number(poly.at(i).at(j).y(), 'f', WKT_P));
            WKTPoly.append(",");
        }

        WKTPoly.append(QString::number( poly.at(i).at(j).x(), 'f', WKT_P));
        WKTPoly.append(" ");
        WKTPoly.append(QString::number( poly.at(i).at(j).y(), 'f', WKT_P));

        if(i < poly.size() - 1)
            WKTPoly.append("),");
        else
            WKTPoly.append(")");
    }

    WKTPoly.append(")'");
    return WKTPoly;
}
