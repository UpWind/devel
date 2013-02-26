#ifndef SIGNSOUND_H
#define SIGNSOUND_H

#include <QObject>
#include "../chartobjectinterface.h"

class SignSound: public ChartObjectInterface
{
    Q_INTERFACES(ChartObjectInterface)

public:
    explicit SignSound(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData,OGRLayer* feat, QString tabName);
    SignSound(QList<ChartObjectInterface::ChartElement> rocks, OGRLayer *feat, QString tabName);
    ~SignSound();
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


#endif // SIGNSOUND_H
