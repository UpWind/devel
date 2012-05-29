#include "point.h"
#include <QHashIterator>
#include <iostream>

Point::Point() : z(0) {

}

Point::~Point() {

}

void Point::put(QString key, QVariant value) {
	properties[key.toLower()] = value;
}

QVariant Point::get(QString key) {
	key = key.toLower();
	if(properties.contains(key)) {
		return properties.value(key);
	} else {
                return QString("10000");
	}
}

QPointF Point::getPoint(){
	return point;
}

void Point::setPoint(QPointF p){
	point = p;
}

float Point::getZ() {
	return z;
}

void  Point::setZ(float z) {
	this->z = z;
}
