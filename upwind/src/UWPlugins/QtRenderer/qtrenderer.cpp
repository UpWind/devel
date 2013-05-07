

#include "qtrenderer.h"
#include "chartwidget.h"
#include "routewidget.h"
#include "../UpWindScene/Scene/boat.h"
#include "../UpWindScene/coreupwindscene.h"
#include <QRect>
#include <QGLWidget>

QtRenderer::QtRenderer() :
    chartWidget(0),
    routeWidget(0),
    boatWidget(0) //271112
  , m_zoomFactor(1.0)
  , m_zoomSpeed(1.15) // magic speed of 15% per zoom click
  , m_followBoat(false)
{}

QtRenderer::~QtRenderer()
{}

QString QtRenderer::getName()
{
    return "QtRenderer";
}

void QtRenderer::ConnectPlugin( UpWindSceneInterface* scene, QWidget* frame, CoreChartProvider* model ){
    CoreViewRenderer::ConnectPlugin(scene, frame, model);
    qDebug()<<Q_FUNC_INFO<<"Chart drawing area size: "<<frame->size();
    QGraphicsScene *graphicsScene = new QGraphicsScene;
    QRectF chartBoundaries = model->getChartBoundaries();
    chartWidget = new ChartGraphicsObject(frame->size());
    routeWidget = new RouteWidget(frame->size(),scene,chartBoundaries);
    boatWidget = new BoatWidget(frame->size(), scene, chartBoundaries);

    graphicsScene->addItem(chartWidget);
    graphicsScene->addItem(routeWidget);
    //271112 Do we need this
//    graphicsScene->addItem(boatWidget);

    Boat *boat = new Boat(frame->size(), chartBoundaries);
    scene->setBoat(boat);

    graphicsScene->addItem(scene->getBoat()->getBoatImage());    // put boat image on screen.
    graphicsScene->addItem(scene->getBoat()->getBoatCompass());
    graphicsScene->addItem(scene->getBoat()->getBoatGPS());
    graphicsScene->addItem(scene->getBoat()->getPortLayline());
    graphicsScene->addItem(scene->getBoat()->getStarBoardLayline());

    connect(scene->getBoat(), SIGNAL(boatPositionChanged()), this, SLOT(handleBoatPositionChanged()));

    boatWidget->setBoat(scene->getBoat());
    boatWidget->updateBoatPosition();

    /*QGraphicsView **/view = new QGraphicsView(graphicsScene, frame);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);

    view->setGeometry(0, 0, frame->size().width(), frame->size().height());
    chartWidget->setModel(model);
    view->lower();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setInteractive(true);
//    view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view->setScene(graphicsScene);

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
    chartWidget->rotateLeft();
    routeWidget->rotateLeft();
}

void QtRenderer::rotateRight()
{
    chartWidget->rotateRight();
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
    chartWidget->setZoomMode(activate);
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

    chartWidget->setZoomFactor(m_zoomFactor);
    routeWidget->setZoomFactor(m_zoomFactor);
    boatWidget->setZoomFactor(m_zoomFactor);

    view->centerOn(viewCenterBeforeScale * scale);
}

void QtRenderer::handleBoatPositionChanged()
{
    if (m_followBoat) {
        view->centerOn(boatWidget->getBoat()->getBoatScenePosition());
    }
}

Q_EXPORT_PLUGIN2(QtRenderer, QtRenderer)
