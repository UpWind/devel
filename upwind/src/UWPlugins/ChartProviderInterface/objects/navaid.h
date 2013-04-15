#ifndef NAVAID_H
#define NAVAID_H

#include <QObject>
#include "../chartobjectinterface.h"
#include<QtGui>

class NavaId: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit NavaId(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    NavaId(QList<ChartObjectInterface::ChartElement> rocks, OGRLayer *feat, QString tabName);
    ~NavaId();
    ChartObjectInterface::ChartObjectType getType() const;
    QVector<QPolygonF> getPixelGeometry() const;
    QVector<QPolygonF> getCoordinateGeometry() const;
    QPen getPen() const;
    QBrush getBrush() const;
    QString getTableName() const;
    OGRLayer* getFeatureData() const;
    const QString resourceName(ChartElement::Attributes attributes) const;

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

#endif // NAVAID_H
