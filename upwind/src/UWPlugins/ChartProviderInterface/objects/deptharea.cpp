#include "deptharea.h"

DepthArea::DepthArea(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

DepthArea::~DepthArea(){}

void DepthArea::setupGraphicProperties() {
    pen.setColor(Qt::white);//"#3b88d0");//#feffa2");
    pen.setWidthF(1);
    brush.setColor(Qt::white);//"#aac9e6");//#feffa2");
}

ChartObjectInterface::ChartObjectType DepthArea::getType() const {
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> DepthArea::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> DepthArea::getCoordinateGeometry() const {
    return cGeometry;
}
QPen DepthArea::getPen() const {
    return pen;
}

QBrush DepthArea::getBrush() const {
    return brush;
}

QString DepthArea::getTableName() const {
    return tableName;
}

OGRLayer* DepthArea::getFeatureData() const {
    return featureData;
}


