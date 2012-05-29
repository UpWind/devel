#include "transmline.h"

TransmissionLine::TransmissionLine(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

TransmissionLine::~TransmissionLine(){}

void TransmissionLine::setupGraphicProperties() {
    pen.setColor(Qt::darkRed);
    pen.setWidthF(2);
    brush.setColor(Qt::black);
}

ChartObjectInterface::ChartObjectType TransmissionLine::getType() const {
    return ChartObjectInterface::Line;
}

QVector<QPolygonF> TransmissionLine::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> TransmissionLine::getCoordinateGeometry() const {
    return cGeometry;
}
QPen TransmissionLine::getPen() const {
    return pen;
}

QBrush TransmissionLine::getBrush() const {
    return brush;
}

QString TransmissionLine::getTableName() const {
    return tableName;
}

OGRLayer* TransmissionLine::getFeatureData() const {
    return featureData;
}


