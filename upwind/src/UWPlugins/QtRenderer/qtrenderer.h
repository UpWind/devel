#ifndef QTRENDERER_H
#define QTRENDERER_H

#include "coreviewrenderer.h"
#include <QObject>
#include "boatwidget.h"

class ChartWidget;
class RouteWidget;
class BoatWidget;

class QtRenderer : public CoreViewRenderer
{
    Q_OBJECT
    Q_INTERFACES(ViewRendererInterface UWPluginInterface)

public:
    QtRenderer();
    ~QtRenderer();

    QString getName();
    void ConnectPlugin(UpWindSceneInterface* scene,QWidget* frame, CoreChartProvider* model);

protected slots:
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
    void zoomBoat();
    void expand();
    void zoomToolActivated(bool);
    void simModeChanged(bool);
    void drawRoute(bool);

private:
    ChartWidget *chartWidget;
    RouteWidget *routeWidget;
    BoatWidget *boatWidget;
};

#endif // QTRENDERER_H
