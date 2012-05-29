

#include "qtrenderer.h"
#include "chartwidget.h"
#include "routewidget.h"
#include "../UpWindScene/Scene/boat.h"
#include "../UpWindScene/coreupwindscene.h"
#include <QRect>

QtRenderer::QtRenderer() :
    chartWidget(0),
    routeWidget(0)
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
    QGraphicsScene *graphicsScene=new QGraphicsScene;
    QRectF chartBoundaries = model->getChartBoundaries();
    chartWidget = new ChartWidget(frame->size());
    routeWidget = new RouteWidget(frame->size(),scene,chartBoundaries);
    boatWidget = new BoatWidget(frame->size(), scene, chartBoundaries);

    graphicsScene->addItem(chartWidget);
    graphicsScene->addItem(routeWidget);
    Boat *boat = new Boat(frame->size(), chartBoundaries);
    scene->setBoat(boat);


    graphicsScene->addItem(scene->getBoat()->getBoatImage());    // put boat image on screen.
    boatWidget->setBoat(scene->getBoat());
    boatWidget->updateBoatPosition();


    QGraphicsView *view=new QGraphicsView(graphicsScene, frame);
    view->setGeometry(0, 0, frame->size().width(), frame->size().height());
    chartWidget->setModel(model);
    view->lower();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setDragMode(QGraphicsView::NoDrag);
}

void QtRenderer::zoomIn()
{
    chartWidget->zoomIn();
    routeWidget->zoomIn();

    boatWidget->zoomIn();
}

void QtRenderer::zoomOut()
{
    chartWidget->zoomOut();
    routeWidget->zoomOut();

    boatWidget->zoomOut();
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
}

void QtRenderer::expand()
{
    chartWidget->expand();
    routeWidget->expand();
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

Q_EXPORT_PLUGIN2(qtrenderer, QtRenderer)
