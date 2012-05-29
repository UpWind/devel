#include "limit.h"


Limit::Limit(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

Limit::~Limit(){}

void Limit::setupGraphicProperties() {
    pen.setColor("#bd0f59");//Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor("#bd0f59");//Qt::black);
}

ChartObjectInterface::ChartObjectType Limit::getType() const {
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> Limit::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> Limit::getCoordinateGeometry() const {
    return cGeometry;
}
QPen Limit::getPen() const {
    return pen;
}

QBrush Limit::getBrush() const {
    return brush;
}

QString Limit::getTableName() const {
    return tableName;
}

OGRLayer* Limit::getFeatureData() const {
    return featureData;
}
