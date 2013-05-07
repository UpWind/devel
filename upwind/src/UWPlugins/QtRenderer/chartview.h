#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QGraphicsView>

class ChartView : public QGraphicsView
{
    Q_OBJECT
public:
    ChartView(QGraphicsScene *scene, QWidget *parent = 0);

protected:
    void wheelEvent(QWheelEvent *event);
};

#endif // CHARTVIEW_H
