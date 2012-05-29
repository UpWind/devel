#include "rock.h"

Rock::Rock(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
     ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

Rock::~Rock(){}

void Rock::setupGraphicProperties() {
    pen.setColor(Qt::black);
    pen.setWidthF(0);
    brush.setColor(Qt::white);

}

ChartObjectInterface::ChartObjectType Rock::getType() const {
    return ChartObjectInterface::Point;
}

QVector<QPolygonF> Rock::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> Rock::getCoordinateGeometry() const {
    return cGeometry;
}
QPen Rock::getPen() const {
    return pen;
}

QBrush Rock::getBrush() const {
    return brush;
}

QString Rock::getTableName() const {
    return tableName;
}

OGRLayer* Rock::getFeatureData() const {
    return featureData;
}

