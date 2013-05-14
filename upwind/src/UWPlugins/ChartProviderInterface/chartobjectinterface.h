#ifndef CHARTOBJECTINTERFACE_H
#define CHARTOBJECTINTERFACE_H

#include <QVector>
#include <QPolygonF>
#include <QPen>
#include <QBrush>
#include <QString>
#include <QHash>
#include <QVariant>

class OGRLayer;

class ChartObjectInterface{
public:
    enum ChartObjectType {
        Point,
        Line,
        Polygon
    } typedef ChartObjectType;

    class ChartElement {
    public:
        typedef QHash<QString, QVariant> Attributes;
        QString name;
        QPointF geoPoint;
        QPointF pixelPoint;
        Attributes attributes;
    };

    explicit ChartObjectInterface(QVector<QPolygonF> geomPixelData, QVector<QPolygonF> geomCoordinateData, OGRLayer* feat, QString tabName)
    {
        pGeometry = geomPixelData;
        cGeometry = geomCoordinateData;
        (void)feat;
        (void)tabName;
    }
    virtual ChartObjectType getType() const = 0;
    virtual QVector<QPolygonF> getPixelGeometry() const = 0;
    virtual QVector<QPolygonF> getCoordinateGeometry() const = 0;
    virtual QPen getPen() const = 0;
    virtual QBrush getBrush() const = 0;
    virtual QString getTableName() const = 0;
    virtual OGRLayer* getFeatureData() const = 0;
    virtual const QString resourceName(ChartElement::Attributes attributes) const {
        Q_UNUSED(attributes); return QString();
    }
    virtual QList<ChartElement> elements() const { return mElements; }

protected:
    QList<ChartElement> mElements;
    QVector<QPolygonF> pGeometry;
    QVector<QPolygonF> cGeometry;
};

#endif // CHARTOBJECTINTERFACE_H
