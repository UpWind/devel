#include "depthareashallow.h"

DepthAreaShallow::DepthAreaShallow(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

DepthAreaShallow::~DepthAreaShallow(){}

void DepthAreaShallow::setupGraphicProperties() {
    pen.setColor("#3b88d0");//#feffa2");
    pen.setWidthF(1);
    brush.setColor("#aac9e6");//#feffa2");
}

ChartObjectInterface::ChartObjectType DepthAreaShallow::getType() const {
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> DepthAreaShallow::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> DepthAreaShallow::getCoordinateGeometry() const {
    return cGeometry;
}
QPen DepthAreaShallow::getPen() const {
    return pen;
}

QBrush DepthAreaShallow::getBrush() const {
    return brush;
}

QString DepthAreaShallow::getTableName() const {
    return tableName;
}

OGRLayer* DepthAreaShallow::getFeatureData() const {
    return featureData;
}


