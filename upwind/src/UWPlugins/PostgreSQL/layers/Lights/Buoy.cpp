#include "Buoy.h"


Buoy::Buoy(const QPointF &position, const QString &name, LightType lightType,
		const QString &buoyImagePath, LightColor lightConeColor, double lightConeRotation)
: position(position), name(name), lightType(lightType), lightConeRotation(lightConeRotation)
{
	buoy.load(buoyImagePath);
	if(buoy.isNull()) {
		qWarning() << "The pixmap from" << buoyImagePath << " is null";
	}

	switch(lightConeColor) {
	case WHITE:
	case YELLOW:
                lightCone.load(":/symbols/yellowcone");
		break;
	case RED:
                lightCone.load(":/symbols/redcone");
		break;
	case GREEN:
                lightCone.load(":/symbols/greencone");
		break;
	default:
		;
	}
}


void Buoy::render(QGraphicsItemGroup *layer) {

	QPointF origo(0,0);

	QGraphicsPixmapItem *buoyItem = new QGraphicsPixmapItem(buoy);
	buoyItem->setPos(origo);

        QRectF bb = buoyItem->boundingRect();
        //buoyItem->rotate(90.0f);
        //buoyItem->translate(-bb.width()/2.0f, -bb.height());
        buoyItem->setOffset(-bb.width()/2, -bb.height()*0.8);
        QGraphicsPixmapItem *coneItem = new QGraphicsPixmapItem(lightCone);
        coneItem->setPos(origo);
        coneItem->rotate(180 + lightConeRotation);
        //coneItem->rotate(lightConeRotation);

        bb = coneItem->boundingRect();
        //coneItem->translate(-bb.width()/2.0f, -bb.height());
        coneItem->setOffset(-bb.width()/2, -bb.height()*0.8);
        QGraphicsItemGroup* group = new QGraphicsItemGroup();

        group->addToGroup(buoyItem);
        group->addToGroup(coneItem);
        //group->scale(0.000025, 0.000025);
        group->scale(0.25, 0.25);
	group->setZValue(6.5);
	group->setPos(position);
	group->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
	//TODO: check if i should give and offset to the pixmaps

	layer->addToGroup(group);
}


