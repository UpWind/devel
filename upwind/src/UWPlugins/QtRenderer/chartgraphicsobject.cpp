#define QT_NO_DEBUG_OUTPUT
//comment
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

#include "corechartprovider.h"
#include "chartobjectinterface.h"
#include "chartgraphicsobject.h"
#include "../shared/uwmath.h"

ChartGraphicsObject::ChartGraphicsObject(QSize size) :
    zoomFactor(1.0),
    rotateAngle(0.0),
    size(size)
  , m_zoomEventReceived(false)
{
    qDebug()<<Q_FUNC_INFO;
    this->setCacheMode(QGraphicsItem::ItemCoordinateCache);
}

ChartGraphicsObject::~ChartGraphicsObject(){}

void ChartGraphicsObject::setModel(CoreChartProvider *model) {
    qDebug()<<Q_FUNC_INFO;

    this->model = model;
    connect(model, SIGNAL(modelChanged()), this, SLOT(fetchChartObjects()));
    model->setAreaFilter(QRect(20,60,5,5));
}

void ChartGraphicsObject::fetchChartObjects() {
    chartObjects = model->getChartObjects();
    update();
}

QRectF ChartGraphicsObject::boundingRect() const {
    return QRectF(0,0, size.width()*zoomFactor, size.height()*zoomFactor);
}

void ChartGraphicsObject::drawChartSymbol(QPainter *painter, const QString &resource, const QPointF &point) {
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

void ChartGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    static bool running = false;

    if (running) {
        qDebug() << "ALERTSKI";
    }

    running = true;
    m_zoomEventReceived = false;

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
//        QApplication::processEvents();
        if (m_zoomEventReceived) {
            qDebug() << "BREAKING ZOOM";
            break;
        }

    }

    QRectF selectionArea(mouseButtonPressedPositition, currentMouseCursorPosition);
    painter->setBrush(Qt::gray);
    painter->setOpacity(0.5);
    painter->drawRect(selectionArea);

    running = false;
}

void ChartGraphicsObject::zoomIn() {
    m_zoomEventReceived = true;
    qDebug() << Q_FUNC_INFO;
    zoomFactor += 0.1;
    prepareGeometryChange();
}

void ChartGraphicsObject::zoomOut() {
    qDebug() << Q_FUNC_INFO;
    zoomFactor -= 0.1;
    prepareGeometryChange();
}

void ChartGraphicsObject::rotateLeft() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle -= 1;
    prepareGeometryChange();
}

void ChartGraphicsObject::rotateRight() {
    qDebug() << Q_FUNC_INFO;
    rotateAngle += 1;
    prepareGeometryChange();
}

void ChartGraphicsObject::expand() {
    resetTransform();
    rotateAngle = 0.0;
    zoomFactor = 1.0;
    prepareGeometryChange();
}

void ChartGraphicsObject::setZoomMode(bool active) {
    zoomMode = active;
}

void ChartGraphicsObject::setZoomFactor(qreal zoomFactor)
{
    m_zoomEventReceived = true;
    this->zoomFactor = zoomFactor;
    prepareGeometryChange();
}

void ChartGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << Q_FUNC_INFO << "Mousepressed";

    if(zoomMode)
        mouseButtonPressedPositition = event->pos();
}

void ChartGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
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

void ChartGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

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
