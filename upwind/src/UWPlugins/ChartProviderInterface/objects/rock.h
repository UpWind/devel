#ifndef ROCK_H
#define ROCK_H

#include <QObject>
#include "../chartobjectinterface.h"

class Rock: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit Rock(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    Rock(QList<ChartElement> rocks, OGRLayer *feat, QString tabName);
    ~Rock();
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

#endif // ROCK_H
