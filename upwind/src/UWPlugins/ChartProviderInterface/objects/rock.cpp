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

Rock::Rock(QList<ChartObjectInterface::ChartElement> rocks, OGRLayer *feat, QString tabName)
    : ChartObjectInterface(QVector<QPolygonF>(), QVector<QPolygonF>(), feat, tabName) {
    tableName = tabName;
    featureData = feat;
    mElements = rocks;
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

const QString Rock::resourceName(ChartObjectInterface::ChartElement::Attributes attributes) const {
    QString resource;
    switch (attributes.value("fi_typrock").toInt()) {
    case 1:
        resource = ":/symbols/20_rock1.png";
        break;
    case 2:
        resource = ":/symbols/20_rock2.png";
        break;
    case 3:
        resource = ":/symbols/20_rock3.png";
        break;
    case 4:
        resource = ":/symbols/20_rock4.png";
        break;
    case 5:
        resource = ":/symbols/20_rock5.png";
        break;
    case 6:
        resource = ":/symbols/20_rock6.png";
        break;
    }
    return resource;
}

