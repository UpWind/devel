#ifndef GENERAREA_H
#define GENERAREA_H

#include <QObject>
#include "../chartobjectinterface.h"

class GenerArea: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit GenerArea(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    ~GenerArea();
    ChartObjectInterface::ChartObjectType getType() const;
    QVector<QPolygonF> getPixelGeometry() const;
    QVector<QPolygonF> getCoordinateGeometry() const;
    QPen getPen() const;
    QBrush getBrush() const;
    QString getTableName() const;
    OGRLayer* getFeatureData() const;

private:
    void setupGraphicProperties();

private:
    OGRLayer* featureData;
    QVector<QPolygonF> pGeometry;
    QVector<QPolygonF> cGeometry;
    QPen pen;
    QBrush brush;
    QString tableName;
};


#endif // GENERAREA_H
