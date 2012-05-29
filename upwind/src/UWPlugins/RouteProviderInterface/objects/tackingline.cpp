#include "tackingline.h"

TackingLine::TackingLine(QVector<QPolygonF> geom, QString tabName) :
    RouteObjectInterface(geom, tabName),
    geometry(geom),
    tableName(tabName) {
    setupGraphicProperties();
}

TackingLine::~TackingLine(){}

void TackingLine::setupGraphicProperties() {
    pen.setColor(Qt::white);
    pen.setWidthF(2);
    pen.setStyle(Qt::DotLine);
    brush.setColor(Qt::white);
}

RouteObjectInterface::RouteObjectType TackingLine::getType() const {
    return RouteObjectInterface::Polygon;
}

QVector<QPolygonF> TackingLine::getGeometry() const {
    return geometry;
}

QPen TackingLine::getPen() const {
    return pen;
}

QBrush TackingLine::getBrush() const {
    return brush;
}

QString TackingLine::getTableName() const {
    return tableName;
}
