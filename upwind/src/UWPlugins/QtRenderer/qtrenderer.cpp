#include "qtrenderer.h"
#include "chartgraphicsobject.h"
#include "routewidget.h"
#include "chartview.h"
#include "../UpWindScene/Scene/boat.h"
#include "../UpWindScene/coreupwindscene.h"
#include <QRect>
#include <QGLWidget>

QtRenderer::QtRenderer() :
    m_chartGraphicsObject(0),
    routeWidget(0),
    boatWidget(0),
    m_zoomFactor(1.0),
    m_zoomSpeed(1.15), // magic speed of 15% per zoom click
    m_followBoat(false)
{}

QtRenderer::~QtRenderer()
{}

QString QtRenderer::getName()
{
    return "QtRenderer";
}

void QtRenderer::ConnectPlugin(UpWindSceneInterface* scene, QWidget* frame, CoreChartProvider* model){
    CoreViewRenderer::ConnectPlugin(scene, frame, model);
    QGraphicsScene *graphicsScene = new QGraphicsScene;
    QRectF chartBoundaries = model->getChartBoundaries();
    m_chartGraphicsObject = new ChartGraphicsObject(frame->size());
    routeWidget = new RouteWidget(frame->size(), scene, chartBoundaries);
    boatWidget = new BoatWidget(frame->size(), scene, chartBoundaries);

    // Make sure the view gets mouse events for dragging
    m_chartGraphicsObject->setAcceptedMouseButtons(Qt::NoButton);
    // Set routepoints with right mouse buttons
    routeWidget->setAcceptedMouseButtons(Qt::RightButton);

    graphicsScene->addItem(m_chartGraphicsObject);
    graphicsScene->addItem(routeWidget);

    Boat *boat = new Boat(frame->size(), chartBoundaries);
    scene->setBoat(boat);

    graphicsScene->addItem(scene->getBoat()->getBoatImage());    // put boat image on screen.
    graphicsScene->addItem(scene->getBoat()->getBoatCompass());
    //graphicsScene->addItem(scene->getBoat()->getBoatGPS());
    graphicsScene->addItem(scene->getBoat()->getPortLayline());
    graphicsScene->addItem(scene->getBoat()->getStarBoardLayline());

    connect(scene->getBoat(), SIGNAL(boatPositionChanged()), this, SLOT(handleBoatPositionChanged()));

    boatWidget->setBoat(scene->getBoat());

    view = new ChartView(graphicsScene, frame);
    connect(view, SIGNAL(wheelUp()), this, SLOT(zoomOut()));
    connect(view, SIGNAL(wheelDown()), this, SLOT(zoomIn()));
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    view->setGeometry(0, 0, frame->size().width(), frame->size().height());
    m_chartGraphicsObject->setModel(model);
    view->lower();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setInteractive(true);
    view->setScene(graphicsScene);

    graphicsScene->setSceneRect(graphicsScene->itemsBoundingRect());

    QPixmapCache::setCacheLimit(1024 * 320);
}

void QtRenderer::setFollowBoat(bool follow)
{
    m_followBoat = follow;
}

bool QtRenderer::getFollowBoat() const
{
    return m_followBoat;
}

void QtRenderer::zoomIn()
{
    setZoomFactor( m_zoomFactor * m_zoomSpeed );
}

void QtRenderer::zoomOut()
{
    setZoomFactor( m_zoomFactor / m_zoomSpeed );
}

void QtRenderer::rotateLeft()
{
    m_chartGraphicsObject->rotateLeft();
    routeWidget->rotateLeft();
}

void QtRenderer::rotateRight()
{
    m_chartGraphicsObject->rotateRight();
    routeWidget->rotateRight();
}

void QtRenderer::zoomBoat()
{
    qreal boatViewZoomFactor = 5.0;
    setZoomFactor(boatViewZoomFactor);
}

void QtRenderer::expand()
{
    setZoomFactor(1.0);
    view->centerOn(QPointF());
}

void QtRenderer::zoomToolActivated(bool activate)
{
    m_chartGraphicsObject->setZoomMode(activate);
    routeWidget->setZoomMode(activate);
}
void QtRenderer::simModeChanged(bool activate)
{
    routeWidget->setSimMode(activate);
}
void QtRenderer::drawRoute(bool activate)
{
    routeWidget->drawRoute(activate);
}

void QtRenderer::setZoomFactor(qreal zoomFactor)
{
    QPointF viewCenterBeforeScale = view->mapToScene(view->rect().center());

    qreal scale = zoomFactor / m_zoomFactor;

    m_zoomFactor = zoomFactor;

    m_chartGraphicsObject->setZoomFactor(m_zoomFactor);
    routeWidget->setZoomFactor(m_zoomFactor);
    boatWidget->setZoomFactor(m_zoomFactor);

    view->centerOn(viewCenterBeforeScale * scale);
    view->setSceneRect(view->scene()->itemsBoundingRect());
}

void QtRenderer::handleBoatPositionChanged()
{
    if (m_followBoat) {
        view->centerOn(boatWidget->getBoat()->getBoatScenePosition());
    }
}
