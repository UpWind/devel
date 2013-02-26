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

NavaId::NavaId(QList<ChartObjectInterface::ChartElement> rocks, OGRLayer *feat, QString tabName)
    : ChartObjectInterface(QVector<QPolygonF>(), QVector<QPolygonF>(), feat, tabName) {
    tableName = tabName;
    featureData = feat;
    mElements = rocks;
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

const QString NavaId::resourceName(ChartObjectInterface::ChartElement::Attributes attributes) const {
    QString resource;
    switch (attributes.value("fi_typenav").toInt()) {
    case 9:
        // buoy
        switch (attributes.value("ntypnavaid").toInt()) {
        case 1:
            //left side mark
            resource = ":/symbols/40_buoy_left.png";
        break;
        case 2:
            //right side mark
            resource = ":/symbols/40_buoy_right.png";
        break;
        case 9:
            // special mark
            resource = ":/symbols/40_buoy_specialmark.png";
        break;
        default:
        break;
        }
    break;
    default:
    break;
    }
    return resource;
}
