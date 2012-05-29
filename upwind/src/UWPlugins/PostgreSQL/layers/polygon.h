
#ifndef POLYGON_H_
#define POLYGON_H_



#include <QPolygonF>
#include <QHash>
#include <QVariant>

class Polygon {
private:
	QPolygonF polygon;
	QHash<QString, QVariant> properties;

public:
	Polygon();
	virtual ~Polygon();

	void put(QString key, QVariant value);
	QVariant get(QString key);

	QPolygonF getPolygon();

	void setPolygon(QPolygonF);
};

#endif /* POLYGON_H_ */
