#include <QDebug>
#include "chartview.h"

ChartView::ChartView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    QGraphicsView::wheelEvent(event);
}
