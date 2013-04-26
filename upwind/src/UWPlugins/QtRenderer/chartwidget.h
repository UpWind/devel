#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QGraphicsItem>
#include "../UpWindScene/Scene/route.h" //tomi: testing

class CoreChartProvider;
class ChartObjectInterface;

class ChartWidget : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit ChartWidget(QSize size);
    ~ChartWidget();
    void setModel(CoreChartProvider *model);
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
    QRectF boundingRect() const;
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
    void expand();
    void setZoomMode(bool);
    void setZoomFactor(qreal zoomFactor);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private slots:
    void fetchChartObjects();

signals:
    void zoomChanged(qreal zoomFactor);

private:
    void drawChartSymbol(QPainter *painter, const QString &resource, const QPointF &point);

    CoreChartProvider *model;
    QVector<ChartObjectInterface*> chartObjects;
    QPointF mouseButtonPressedPositition;
    QPointF currentMouseCursorPosition;
    qreal zoomFactor;
    qreal rotateAngle;
    QSize size;
    Route *route;

    bool zoomMode;
};

#endif // CHARTWIDGET_H
