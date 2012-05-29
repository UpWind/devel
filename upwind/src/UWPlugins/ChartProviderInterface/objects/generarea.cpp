#include "generarea.h"

GenerArea::GenerArea(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

GenerArea::~GenerArea(){}

void GenerArea::setupGraphicProperties() { 
    pen.setColor("#877128");//Qt::yellow);
    pen.setWidthF(1);
    brush.setColor("#FDCA5F");//Qt::yellow);
}

ChartObjectInterface::ChartObjectType GenerArea::getType() const {
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> GenerArea::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> GenerArea::getCoordinateGeometry() const {
    return cGeometry;
}
QPen GenerArea::getPen() const {
    return pen;
}

QBrush GenerArea::getBrush() const {
    return brush;
}

QString GenerArea::getTableName() const {
    return tableName;
}

OGRLayer* GenerArea::getFeatureData() const {
    return featureData;
}
