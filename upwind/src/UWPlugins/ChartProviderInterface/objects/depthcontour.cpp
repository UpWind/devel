#include "depthcontour.h"

DepthContour::DepthContour(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

DepthContour::~DepthContour(){}

void DepthContour::setupGraphicProperties() {
    pen.setColor("#3b88d0");//Qt::white);
    pen.setWidthF(1);
    brush.setColor("#3b88d0");//Qt::white);
}

ChartObjectInterface::ChartObjectType DepthContour::getType() const {
    return ChartObjectInterface::Line;
}

QVector<QPolygonF> DepthContour::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> DepthContour::getCoordinateGeometry() const {
    return cGeometry;
}
QPen DepthContour::getPen() const {
    return pen;
}

QBrush DepthContour::getBrush() const {
    return brush;
}

QString DepthContour::getTableName() const {
    return tableName;
}

OGRLayer* DepthContour::getFeatureData() const {
    return featureData;
}


