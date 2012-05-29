#include "coastline.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QDebug>
#include <QRect>

#include "PostgreChartProvider/postgrechartprovider.h"

CoastLine::CoastLine(){
    pg = new PostgreChartProvider("localhost", 5432, "chart57", "postgres", "postgres");
    connect(pg, SIGNAL(modelChanged()), this, SLOT(refresh()));
    pg->setAreaFilter(QRect(20,60,5,5));
}

void CoastLine::refresh()
{
    update();
}

void CoastLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qDebug() << Q_FUNC_INFO;
    QBrush coast_brush = QBrush(Qt::yellow);

    painter->setBrush(coast_brush);
    QVector<QPolygonF> geometry = pg->getLayersGeometry().value("generarea_r_level5");
    foreach(QPolygonF poly, geometry){
        painter->drawPolygon(poly);
    }

    QBrush depth_brush = QBrush(Qt::blue);

    painter->setBrush(depth_brush);
    geometry = pg->getLayersGeometry().value("deptharea_r_level5");
    foreach(QPolygonF poly, geometry){
        painter->drawPolygon(poly);
    }
}

QRectF CoastLine::boundingRect() const
{
    return QRectF(0,0,400,400);
}
