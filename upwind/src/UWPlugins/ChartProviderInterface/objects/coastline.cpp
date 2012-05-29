#include "coastline.h"

CoastLine::CoastLine(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName) :
    ChartObjectInterface(geomPixelData,geomCoordinateData,feat,tabName),
    pGeometry(geomPixelData),
    cGeometry(geomCoordinateData),
    featureData(feat),
    tableName(tabName)
{
    setupGraphicProperties();
}

CoastLine::~CoastLine(){}

void CoastLine::setupGraphicProperties() {
    pen.setColor("#877128");//#bdd49e");
    pen.setWidthF(1);
    brush.setColor("#ffcf6a");//#bdd49e");
}

ChartObjectInterface::ChartObjectType CoastLine::getType() const {
    return ChartObjectInterface::Polygon;
}

QVector<QPolygonF> CoastLine::getPixelGeometry() const {
    return pGeometry;
}

QVector<QPolygonF> CoastLine::getCoordinateGeometry() const {
    return cGeometry;
}
QPen CoastLine::getPen() const {
    return pen;
}

QBrush CoastLine::getBrush() const {
    return brush;
}

QString CoastLine::getTableName() const {
    return tableName;
}

OGRLayer* CoastLine::getFeatureData() const {
    return featureData;
}

