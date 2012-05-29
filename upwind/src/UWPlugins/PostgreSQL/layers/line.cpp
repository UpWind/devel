#include "line.h"

#include <QHashIterator>

Line::Line() {

}

Line::~Line() {

}

void Line::put(QString key, QVariant value) {
	properties[key.toLower()] = value;
}

QVariant Line::get(QString key) {
	key = key.toLower();
	if(properties.contains(key)) {
		return properties.value(key);
	} else {
		return QString("Debug");
	}
}

QLineF Line::getLine(){
	return line;
}

void Line::setLine(QLineF l){
	line = l;
}
