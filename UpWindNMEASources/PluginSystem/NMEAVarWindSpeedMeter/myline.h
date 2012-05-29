#ifndef MYLINE_H
#define MYLINE_H

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainterPath>

class MyLine: public QObject, public QGraphicsItem
{
Q_OBJECT
public:
    //MyLine(QGraphicsItem *parent = 0);
    MyLine();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void passElements(QList<QPointF>  );//concersns this is a public functin , is it ok? i am accessing from view
    //this passelements is passing the wind speed pixel values to line
    //void passElements(QPainterPath path);//concersns this is a public functin , is it ok? i am accessing from view
protected:
    void advance(int phase);
private:
    qreal angle;
    int savetimervalue;
    QString interval;
    qreal speed;
    void DoCollision();
    QList<QPointF> listofpoint;
    QPainterPath p;
    int adv;
    int timer;
    QList<qreal> windspeeds;
    bool start;
signals:
    void signal(qreal);
    void sendNewDialValue();
private slots:
    //void valuesReady(QList<QPointF> p);
public:
    //qreal getWindSpeeds();

};
#endif // MYLINE_H
