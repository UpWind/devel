#include "wreck.h"

Wreck::Wreck(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
    ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

Wreck::~Wreck(){}

void Wreck::setupGraphicProperties() {
    pen.setColor(Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor(Qt::black);
}

ChartObjectInterface::ChartObjectType Wreck::getType() const {
    return ChartObjectInterface::Point;
}

QVector<QPolygonF> Wreck::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> Wreck::getCoordinateGeometry() const {
    return cGeometry;
}
QPen Wreck::getPen() const {
    return pen;
}

QBrush Wreck::getBrush() const {
    return brush;
}

QString Wreck::getTableName() const {
    return tableName;
}

OGRLayer* Wreck::getFeatureData() const {
    return featureData;
}
