#include "navaid.h"


NavaId::NavaId(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
     ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

NavaId::~NavaId(){}

void NavaId::setupGraphicProperties() {
    pen.setColor(Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor(Qt::black);
}

ChartObjectInterface::ChartObjectType NavaId::getType() const {
    return ChartObjectInterface::Point;
}

QVector<QPolygonF> NavaId::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> NavaId::getCoordinateGeometry() const {
    return cGeometry;
}
QPen NavaId::getPen() const {
    return pen;
}

QBrush NavaId::getBrush() const {
    return brush;
}

QString NavaId::getTableName() const {
    return tableName;
}

OGRLayer* NavaId::getFeatureData() const {
    return featureData;
}
