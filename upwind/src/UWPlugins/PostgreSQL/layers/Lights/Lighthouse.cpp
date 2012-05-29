#include "Lighthouse.h"


Lighthouse::Lighthouse(const QPointF &position, const QString &name,
		const QString &symbolPath,
		LightType lightType, qint32 number,
		qint16 officialCharacter, qint16 exactCharacter)
: position(position), name(name), symbolPath(symbolPath),
lightType(lightType), number(number),
officialCharacter(officialCharacter), exactCharacter(exactCharacter),
maxRange(0.0) {}




void Lighthouse::addSector(Point &sectorInfo) {
	Sector sector(sectorInfo);
	sectors.append(sector);
}

int Lighthouse::sectorCount() const {
	return sectors.count();
}

void Lighthouse::render(QGraphicsItemGroup *layer) {
        const float ROTATION = 90.0f;

	QGraphicsItemGroup* group = new QGraphicsItemGroup();


	QPointF origo(0.0f, 0.0f);

	// TODO This is a quick fix for lights who want to draw themselves off the chart.
	//		Come up with a real solution!
	/*if(position == origo) {
		static int counter = 0;
		//std::cout << "Couldn't load: " << ++counter << std::endl;
		return ;
	}*/

	QPixmap symbol(symbolPath);
	QGraphicsPixmapItem *symbolItem = new QGraphicsPixmapItem(symbol);
	//symbolItem->setToolTip("Some info might be here");
        //symbolItem->scale(0.000015, 0.000015);

	symbolItem->setZValue(6.0f);
        symbolItem->scale(0.25,0.25);

	QRectF bb = symbolItem->boundingRect();
        symbolItem->translate(-bb.width()/2.0f, -bb.height()*0.8);
        //symbolItem->scale(0.0005, 0.0005);
        //group->addToGroup(symbolItem);
        QGraphicsItemGroup* fix = new QGraphicsItemGroup();

        fix->setPos(position.x(), position.y());
        fix->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
        fix->addToGroup(symbolItem);

        QPen linePen(Qt::DotLine);
	QGraphicsLineItem *lineItem = 0;
	QLineF line;

        const float initialRadius = 0.3f; // of the sectors
        const double arcWidth = 0.02;
        const float addition = 0.2f; // addition to the lenght of dashed line

	for(QList<Sector>::const_iterator sector = sectors.begin();
		sector != sectors.end(); ++sector ) {
		// calculate the properties for sectors
		const double startAngle = sector->getStartAngle();
		const double endAngle = sector->getEndAngle();

		double diffAngle = 0.0;
		if(endAngle >= startAngle) {
			diffAngle = endAngle - startAngle;
		} else {
			diffAngle = (endAngle + 360) - startAngle;
		}

		float radius = 0.0f;
		float lineLenght = 0.0f;
		if(diffAngle < 3 ) {
			radius = initialRadius  * 3.35;
			lineLenght = radius + addition;
		} else if(diffAngle < 9) {
			radius = initialRadius  * 3.00;
			lineLenght = radius + addition;
		} else if(diffAngle < 30) {
			radius = initialRadius  * 2.33;
			lineLenght = radius + addition;
		} else if(diffAngle < 45) {
			radius = initialRadius  * 2;
			lineLenght = radius + addition;
		} else if(diffAngle < 60) {
			radius = initialRadius  * 1.75;
			lineLenght = radius + addition;
		} else if(diffAngle < 90) {
			radius = initialRadius  * 1.5;
			lineLenght = radius + addition;
		} else if(diffAngle < 120) {
			radius = initialRadius  * 1.25;
			lineLenght = radius + addition;
		} else {
			radius = initialRadius;
			lineLenght = radius + addition;
		}
		if(diffAngle == 360.0f) {
                        radius = (0.05f)  * 1.25;
			lineLenght = radius + addition;
		}

		// == First dashed line ==
		QPointF lineEnd;
		lineEnd.setX( lineLenght * cos(degreesToRadians(startAngle)) );
		lineEnd.setY( lineLenght * sin(degreesToRadians(startAngle)) );

		line.setP1(origo);
		line.setP2(lineEnd);

		lineItem = new QGraphicsLineItem(line);
		lineItem->setPen(linePen);

		lineItem->moveBy(position.x(), position.y());
		lineItem->rotate(ROTATION);
		lineItem->setZValue(4.0f);
		lineItem->scale(0.0005, 0.0005);
		group->addToGroup(lineItem);

		// == light sector ==
		QGraphicsItem *sectorItem = sector->render(radius, arcWidth);

		sectorItem->moveBy(position.x(), position.y());
		sectorItem->rotate(ROTATION);
		sectorItem->setZValue(4.0f);
		sectorItem->scale(0.0005, 0.0005);
		group->addToGroup(sectorItem);

		// == second dashed line ==
		lineEnd.setX( lineLenght * cos(degreesToRadians(endAngle)) );
		lineEnd.setY( lineLenght * sin(degreesToRadians(endAngle)) );

		line.setP1(origo);
		line.setP2(lineEnd);
		lineItem = new QGraphicsLineItem(line);
		lineItem->setPen(linePen);

		lineItem->moveBy(position.x(), position.y());
		lineItem->rotate(ROTATION);
		lineItem->setZValue(4.0f);
		lineItem->scale(0.0005, 0.0005);
		group->addToGroup(lineItem);

		// == the letter ==
		double temp = radius + arcWidth + 2;
		QLineF helperLine(position.x(), position.y(), position.x() + temp, position.y());

		double midAngle = 0.0;
		if(startAngle > endAngle ) {
			double temp = (endAngle + (360.0 - startAngle))/2.0;
			midAngle = startAngle + temp;
		} else {
			midAngle = (endAngle + startAngle)/2.0;
		}

		helperLine.setAngle(-midAngle + ROTATION);

		QString letter = ColorToLetter(sector->getColor());
		QGraphicsSimpleTextItem *textItem = new QGraphicsSimpleTextItem(letter);

		textItem->setPos(helperLine.p2());
		textItem->rotate(90.0f);

		QRectF bb = textItem->boundingRect();
		textItem->translate(-bb.width()/2.0f, -bb.height()/2.0f);
		textItem->setZValue(6.0f);
		textItem->scale(0.0005, 0.0005);
		group->addToGroup(textItem);
	}

	group->setZValue(6.0f);

        layer->addToGroup(fix);
	layer->addToGroup(group);
}
