#include <QDebug>
#include <QWheelEvent>
#include "chartview.h"

ChartView::ChartView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    if (event->delta() > 0 ) {
        emit wheelDown();
    } else if (event->delta() < 0 ) {
        emit wheelUp();
    }

    event->accept();

//    QGraphicsView::wheelEvent(event);
}
