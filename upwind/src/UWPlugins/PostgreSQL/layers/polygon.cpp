#include "polygon.h"
#include <QHashIterator>

Polygon::Polygon() {

}

Polygon::~Polygon() {

}

void Polygon::put(QString key, QVariant value) {
	properties[key.toLower()] = value;
}

QVariant Polygon::get(QString key) {
	key = key.toLower();
	if(properties.contains(key)) {
		return properties.value(key);
	} else {
		return QString("Debug");
	}
}

QPolygonF Polygon::getPolygon() {
	return polygon;
}

void Polygon::setPolygon(QPolygonF p){
	polygon=p;
}
