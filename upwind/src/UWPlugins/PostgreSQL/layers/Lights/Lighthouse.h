
#ifndef LIGHTHOUSE_H_
#define LIGHTHOUSE_H_

#include <QList>
#include <QString>
#include <QPointF>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QPixmap>
#include <QGraphicsItem>
#include <math.h>

#include "Utilies.h"
#include "Sector.h"
#include "Light.h"


class Sector;
class Point;
class QString;
class QPixmap;
class QPointF;
class QGraphicsItemGroup;

/* NOTE:
 * Lighthouse doesn't store ogr_fid or se_cdate fields at all
 */
class Lighthouse : public Light
{
public:
	Lighthouse(const QPointF &position, const QString &name,
			const QString &symbolPath,
			LightType ligtType, qint32 number,
			qint16 officialCharacter, qint16 exactCharacter);
        virtual ~Lighthouse() {}

	void addSector(Point &sectorInfo);

	int sectorCount() const;

	// draws the lighthouse in the scene
	void render(QGraphicsItemGroup *layer);

	QString getName() const { return name; }
	LightType getLightType() const { return lightType; }

	qint16 getOfficialCharacter() const { return officialCharacter; }
	qint16 getExactCharacter() const { return exactCharacter; }
	qint32 getNumber() const { return number; }
	QPointF getPosition() const { return position; }

private:
	QList<QGraphicsItem *> graphicsItems;

	QPointF position;
	QString name; // navaidoid?
	QString symbolPath;

	LightType lightType;
	qint32 number; // it has 9 digits
	// characters have 5 digits each
	qint16 officialCharacter; // virallinen tunnus, FI_offlchr
	qint16 exactCharacter; // tarkkatunnus, FI_prelchr
	QList<Sector> sectors;

	// implementation
	double maxRange;
};

#endif
