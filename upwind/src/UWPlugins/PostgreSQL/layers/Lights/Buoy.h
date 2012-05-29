
#ifndef BUOY_H_
#define BUOY_H_

#include "Light.h"
#include "Utilies.h"
#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

class QGraphicsItemGroup;



class Buoy : public Light
{
public:
	Buoy(const QPointF &position, const QString &name,  LightType lightType,
			const QString  &buoyImagePath,
			LightColor lightConeColor, double lightConeRotation);

        ~Buoy(){}
	void render(QGraphicsItemGroup *layer);

	QString getName() const { return name; }
	LightType getLightType() const { return lightType; }

private:
	QPointF position;
	QString name;
	LightType lightType;
	QPixmap buoy, lightCone;
	double lightConeRotation;
};

#endif
