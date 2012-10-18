#ifndef CHARTOBJECTINTERFACE_H
#define CHARTOBJECTINTERFACE_H

#include <QVector>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QString>


//#include "ogrsf_frmts.h"

class OGRLayer;

class ChartObjectInterface{
public:
    enum ChartObjectType {
        Point,
        Line,
        Polygon
    } typedef ChartObjectType;

    explicit ChartObjectInterface(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName)
    {
        (void)geomPixelData;
        (void)geomCoordinateData;
        (void)feat;
        (void)tabName;
    }
    virtual ChartObjectType getType() const = 0;
    virtual QVector<QPolygonF> getPixelGeometry() const = 0;
    virtual QVector<QPolygonF> getCoordinateGeometry() const = 0;
    virtual QPen getPen() const = 0;
    virtual QBrush getBrush() const = 0;
    virtual QString getTableName() const = 0;
    virtual OGRLayer* getFeatureData() const = 0;
};

#endif // CHARTOBJECTINTERFACE_H
