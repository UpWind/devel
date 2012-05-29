#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QTimer>
#include <QPainterPath>
#include <QDial>
#include <QtGui>
#include "myline.h"

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
  //  void paintEvent(QPaintEvent *event);
public:
    explicit MyQGraphicsView(QWidget *parent = 0);
    void passSimulationTime(int simutime);
signals:
    void valuesReady(QList<QPointF> );
    void passToMainWindow(qreal);
    //void passToMainWindowDials(QList<QDial *>);
    void sendDialValueToWindow(QDial *);

public slots:
    void mousePressEvent(QMouseEvent * e);
    void startTimer();
    void SendNewDialValueToWindow();
    //void setValuesOfList(QList<QDial *> &p);
private slots:
    void windspeedFromLine(qreal);

    // void mouseReleaseEvent(QMouseEvent * e);
    // void mouseDoubleClickEvent(QMouseEvent * e);
    // void mouseMoveEvent(QMouseEvent * e);
private:
    QGraphicsScene * scene;
    QTimer *timer;
    QGroupBox *horizontalGroupBox;
    QGroupBox *horizontalGroupBoxspinboxes;
     QList<QSpinBox *> spinboxlist;
    //QPainterPath path;
    QList<QPointF> listofpoints;
    int noofperiods;
QPainter *painter;
    MyLine *lineitem;
    QList<QDial *> list;
    int initialdial;
};

#endif // MYGRAPHICSVIEW_H



