#include "Sector.h"



Sector::Sector(Point &segmentInfo) {
	sectorNum = segmentInfo.get("SECTORNUM").toInt();
	startAngle = segmentInfo.get("STARTANG").toDouble();
	endAngle = segmentInfo.get("CLOANGLE").toDouble();
	color = (LightColor) (segmentInfo.get("COLORLIGHT").toInt());
	opticalRange = segmentInfo.get("FI_OPTRANG").toDouble();
	range = segmentInfo.get("NOMRANLGHT").toDouble();
	height = segmentInfo.get("HGHTLIGHT").toDouble();
	type = (Type) (segmentInfo.get("TYPELIGHTlight").toDouble());
	specialFunction = (SpecialFunction) (segmentInfo.get("SPEFUNCLHT").toInt());
	visibility = (Visibility) (segmentInfo.get("LIGHTVIS").toInt());

	// sanity asserts based on information from "HIS Kohdemalli.chm"
	Q_ASSERT(startAngle >= 0.0 && startAngle < 360.0);
	Q_ASSERT(endAngle > 0.0 && endAngle <= 360);
}

Sector::~Sector() {
}

QGraphicsItem *Sector::render(double radius, double arcWidth) const {
	QPen pen;
	QBrush brush;

	ColorToBrushAndPen(color, pen, brush);

	QPolygonF polygon = createPolygonArc(radius, arcWidth, startAngle, endAngle);

	QGraphicsPolygonItem *polygonItem = new QGraphicsPolygonItem(polygon);
	polygonItem->setFillRule(Qt::OddEvenFill);
	polygonItem->setPen(pen);
	polygonItem->setBrush(brush);

	return polygonItem;
}

/*
===================================================

Creates an arc shaped polygon. First it creates vertices
for inner side of the arc and the for the outer side.
After creating of an vertex, it's put in to a polygon

===================================================
 */
QPolygonF Sector::createPolygonArc(double radius, double arcWidth,
								   double startAngle, double endAngle) const {
	QPolygonF arc;
	QPointF point;

	double spanAngle = 0.0;
	if(endAngle >= startAngle) {
		spanAngle = endAngle - startAngle;
	} else {
		spanAngle = (endAngle + 360) - startAngle;
	}

	// inner edge of the polygon
	for(double angle = startAngle; angle <= (startAngle + spanAngle); ++angle) {
		point.setX(radius * cos(degreesToRadians(angle)));
		point.setY(radius * sin(degreesToRadians(angle)));
		arc << point;
	}

	// outer edge
	double angle = 0.0;
	if(startAngle + spanAngle > 360) {
		angle = 360.0f + endAngle;
	} else {
		angle = endAngle;
	}

	for( ; angle >= startAngle; --angle) {
		point.setX((arcWidth + radius) * cos(degreesToRadians(angle)));
		point.setY((arcWidth + radius) * sin(degreesToRadians(angle)));
		arc << point;
	}

	return arc;
}

