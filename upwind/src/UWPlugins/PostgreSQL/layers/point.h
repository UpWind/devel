
#ifndef POINT_H_
#define POINT_H_


#include <QPointF>
#include <QHash>
#include <QVariant>

class Point {
private:
	QPointF point;

	QHash<QString, QVariant> properties;

	float z; // currently only used by signsound_p layer
public:
	Point();
	virtual ~Point();

	void put(QString key, QVariant value);
	QVariant get(QString key);

	QPointF getPoint();
	void setPoint(QPointF);

	float getZ();
	void  setZ(float z);
};

#endif /* POINT_H_ */
