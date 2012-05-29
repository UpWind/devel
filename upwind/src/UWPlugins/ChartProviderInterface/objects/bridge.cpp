#include "bridge.h"

Bridge::Bridge(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
    ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

Bridge::~Bridge(){}

void Bridge::setupGraphicProperties() {
    pen.setColor("#666666");//Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor("#666666");//Qt::black);
}

ChartObjectInterface::ChartObjectType Bridge::getType() const {  //not sure if bridge is polygon or line
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> Bridge::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> Bridge::getCoordinateGeometry() const {
    return cGeometry;
}
QPen Bridge::getPen() const {
    return pen;
}

QBrush Bridge::getBrush() const {
    return brush;
}

QString Bridge::getTableName() const {
    return tableName;
}

OGRLayer* Bridge::getFeatureData() const {
    return featureData;
}
