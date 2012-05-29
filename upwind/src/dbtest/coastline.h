#ifndef COASTLINE_H
#define COASTLINE_H

#include <QGraphicsItem>
#include <QObject>
class PostgreChartProvider;

class CoastLine : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit CoastLine();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

signals:

public slots:
private slots:
    void refresh();

private:
    PostgreChartProvider * pg;

};

#endif // COASTLINE_H
