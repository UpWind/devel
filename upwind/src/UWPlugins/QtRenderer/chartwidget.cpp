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
    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}

void ChartWidget::drawChartSymbol(QPainter *painter, const QString &resource, const QPointF &point) {
    painter->save();

    /*
     * This reverts the scale operation affecting the whole chart item.
     * This is needed to keep the size of navigation symbols the same
     * when the overall scale of the chart item changes.
     */
    if (zoomFactor > 0) {
        painter->scale(1 / zoomFactor, 1 / zoomFactor);
    }

    QPixmap pixmap(resource);
    QPointF correctedPoint((point.x() * zoomFactor) - pixmap.width() / 2,
                (point.y() * zoomFactor) - pixmap.height() / 2);

    painter->drawPixmap(correctedPoint, pixmap);

    painter->restore();
}

void ChartWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->scale(zoomFactor, zoomFactor);
    painter->rotate(rotateAngle);

    foreach (ChartObjectInterface *object, chartObjects) {

        QPen pen = object->getPen();
        pen.setWidthF(pen.widthF() / zoomFactor);
        painter->setPen(pen);
        painter->setBrush(object->getBrush().color());
        QVector<QPolygonF> geometry = object->getPixelGeometry();

        switch (object->getType()) {
        case ChartObjectInterface::Point:
            foreach(ChartObjectInterface::ChartElement e, object->elements()) {
                QString resource = object->resourceName(e.attributes);
                if (!resource.isEmpty()) {
                    drawChartSymbol(painter, resource, QPointF(e.pixelPoint.x(), e.pixelPoint.y()));
                }
            }
        break;
        case ChartObjectInterface::Line:
            foreach(QPolygonF poly, geometry)
                painter->drawPolyline(poly);
        break;
        case ChartObjectInterface::Polygon:
            foreach(QPolygonF poly, geometry)
                painter->drawPolygon(poly);
        break;
        default:
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

	(void)event;

    qDebug() << Q_FUNC_INFO << "mouseMoveEvent";


    if(zoomMode) {
        mouseButtonPressedPositition = QPoint(-1,-1);
        currentMouseCursorPosition = QPoint(-1, -1);
        zoomFactor += 0.1;
        prepareGeometryChange();
        update();
    }
}
