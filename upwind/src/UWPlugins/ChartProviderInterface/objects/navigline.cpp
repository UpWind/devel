#include "navigline.h"

NavigLine::NavigLine(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
      ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

NavigLine::~NavigLine(){}

void NavigLine::setupGraphicProperties() {
    pen.setColor("#FF0000");//Qt::black);
    pen.setWidthF(1);
    brush.setColor(Qt::transparent);//"#FF0000");//Qt::black);

}

ChartObjectInterface::ChartObjectType NavigLine::getType() const {
    return ChartObjectInterface::Line;
}

QVector<QPolygonF> NavigLine::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> NavigLine::getCoordinateGeometry() const {
    return cGeometry;
}
QPen NavigLine::getPen() const {
    return pen;
}

QBrush NavigLine::getBrush() const {
    return brush;
}

QString NavigLine::getTableName() const {
    return tableName;
}

OGRLayer* NavigLine::getFeatureData() const {
    return featureData;
}

