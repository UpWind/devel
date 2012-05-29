#ifndef NAVIGLINE_H
#define NAVIGLINE_H

#include <QObject>
#include "../chartobjectinterface.h"

class NavigLine: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit NavigLine(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    ~NavigLine();
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


#endif // NAVIGLINE_H
