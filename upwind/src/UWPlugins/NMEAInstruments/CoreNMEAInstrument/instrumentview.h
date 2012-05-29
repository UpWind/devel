#ifndef INSTRUMENTVIEW_H
#define INSTRUMENTVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QtGui>

class InstrumentView: public QGraphicsView
{
    Q_OBJECT
public:
    InstrumentView(QWidget *parent = 0);

signals:
    void mouseMoved(QMouseEvent *event);
    void mousePressed(QMouseEvent *event);
    void mouseDoubleClicked(QMouseEvent *event);

private:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent (QMouseEvent *event);
    void mouseReleaseEvent (QMouseEvent *event);

    bool m_handling;
};

#endif // INSTRUMENTVIEW_H
