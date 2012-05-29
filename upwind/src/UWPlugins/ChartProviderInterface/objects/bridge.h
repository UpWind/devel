#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>
#include "../chartobjectinterface.h"

class Bridge: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit Bridge(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    ~Bridge();
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


#endif // BRIDGE_H
