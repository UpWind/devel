
#ifndef SECTOR_H_
#define SECTOR_H_

#include <QPen>
#include <QBrush>
#include <QBrush>
#include <QColor>

#include <QPolygonF>
#include <QPointF>
#include <QGraphicsItem>
#include "Utilies.h"
#include "../point.h"
#include <math.h>


// holds the information for a light segment
class Sector
{
public:
	Sector(Point &segmentInfo);
	virtual ~Sector();

	QGraphicsItem *render(double radius, double arcWidth) const;

	// for the properties of the sector
    int getSectorNum() const { return sectorNum; }
    double getStartAngle() const { return startAngle; }
    double getEndAngle() const { return endAngle; }
    LightColor getColor() const { return color; }
    double getOpticalRange() const { return opticalRange; }
    double getRange() const { return range; }
    double getHeight() const { return height; }
    Type getType() const { return type; }
    int getQuality() const { return quality; }
    SpecialFunction getSpecialFunction() const { return specialFunction; }
    Visibility getVisibility() const { return visibility; }

private:
	QPolygonF createPolygonArc(double radius, double arcWidth, double startAngle, double endAngle) const;

private:
    int sectorNum;
    double startAngle, endAngle;
    LightColor color;
    double opticalRange; // FI_optrang
    double range; // nomranlght (in mpk)
    double height; // hghtlight
    Type type; // typelight | p�iv�- vai y�loisto
    int quality; // FI_natlght (TODO rename the var)
    SpecialFunction specialFunction; //spefunclht
    Visibility visibility;
};

#endif
