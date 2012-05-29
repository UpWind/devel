#define QT_NO_DEBUG_OUTPUT
//comment
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "corechartprovider.h"
#include "chartobjectinterface.h"
#include "chartwidget.h"
#include "../shared/uwmath.h"

ChartWidget::ChartWidget(QSize size) :
    zoomFactor(1.0),
    rotateAngle(0.0),
    size(size)
{
    qDebug()<<Q_FUNC_INFO;
    this->setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

ChartWidget::~ChartWidget(){}

void ChartWidget::setModel(CoreChartProvider *model) {
    qDebug()<<Q_FUNC_INFO;

    this->model = model;
    connect(model, SIGNAL(modelChanged()), this, SLOT(fetchChartObjects()));
    model->setAreaFilter(QRect(20,60,5,5));
}

void ChartWidget::fetchChartObjects() {
    chartObjects = model->getChartObjects();
    update();
}

QRectF ChartWidget::boundingRect() const {
    qDebug() << Q_FUNC_INFO;

    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}

void ChartWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    qDebug()<<Q_FUNC_INFO;
    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);
    foreach(ChartObjectInterface *object, chartObjects){
        QPen pen = object->getPen();
        pen.setWidthF(pen.widthF()/zoomFactor);
        painter->setPen(pen);
        painter->setBrush(object->getBrush().color());
        QVector<QPolygonF> geometry = object->getPixelGeometry();

        switch(object->getType()){
        case ChartObjectInterface::Point:
//            qDebug() << Q_FUNC_INFO << "Drawing points";
            foreach(QPolygonF poly, geometry)
                foreach(QPointF point, poly)
                    painter->drawEllipse(point, 4, 4);
            break;
        case ChartObjectInterface::Line:
//            qDebug() << Q_FUNC_INFO << "Drawing lines";
            foreach(QPolygonF poly, geometry)
                painter->drawPolyline(poly);
            break;
        case ChartObjectInterface::Polygon:
//            qDebug() << Q_FUNC_INFO << "Drawing polygons";
            foreach(QPolygonF poly, geometry)
                painter->drawPolygon(poly);
            break;
        default:
            qDebug() << Q_FUNC_INFO << "ChartObject not found.";
            break;
        }
    }

    QRectF selectionArea(mouseButtonPressedPositition, currentMouseCursorPosition);
    painter->setBrush(Qt::gray);
    painter->setOpacity(0.5);
    painter->drawRect(selectionArea);
}

void ChartWidget::zoomIn() {
    qDebug() << Q_FUNC_INFO;
    zoomFactor += 0.1;
    prepareGeometryChange();
}

void ChartWidget::zoomOut() {
    qDebug() << Q_FUNC_INFO;
    zoomFactor -= 0.1;
    prepareGeometryChange();
}

void ChartWidget::rotateLeft() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle -= 1;
    prepareGeometryChange();
}

void ChartWidget::rotateRight() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle += 1;
    prepareGeometryChange();
}

void ChartWidget::expand() {
    resetTransform();
    rotateAngle = 0.0;
    zoomFactor = 1.0;
    prepareGeometryChange();
}

void ChartWidget::setZoomMode(bool active) {
    zoomMode = active;
}

void ChartWidget::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << Q_FUNC_INFO << "Mousepressed";

    if(zoomMode)
        mouseButtonPressedPositition = event->pos();
}

void ChartWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << Q_FUNC_INFO << "mouseMoveEvent";
    if(zoomMode) {
        currentMouseCursorPosition = event->pos();
        QRectF repaintArea;
        if(mouseButtonPressedPositition.x() > currentMouseCursorPosition.x()) {
            repaintArea.setTopLeft(currentMouseCursorPosition);
            repaintArea.setBottomRight(mouseButtonPressedPositition);
        } else {
            repaintArea.setTopLeft(mouseButtonPressedPositition);
            repaintArea.setBottomRight(currentMouseCursorPosition);
        }
        update(repaintArea);
    }
}

void ChartWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << Q_FUNC_INFO << "mouseMoveEvent";


    if(zoomMode) {
        mouseButtonPressedPositition = QPoint(-1,-1);
        currentMouseCursorPosition = QPoint(-1, -1);
        zoomFactor += 0.1;
        prepareGeometryChange();
        update();
    }
}
