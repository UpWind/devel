#ifndef QTRENDERER_H
#define QTRENDERER_H

#include "coreviewrenderer.h"
#include <QObject>
#include "boatwidget.h"

class ChartGraphicsObject;
class RouteWidget;
class BoatWidget;

class QtRenderer : public CoreViewRenderer
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.QtRenderer")

public:
    QtRenderer();
    ~QtRenderer();

    QString getName();
    void ConnectPlugin(UpWindSceneInterface* scene,QWidget* frame, CoreChartProvider* model);

    bool getFollowBoat() const;

public slots:
    void setFollowBoat(bool follow);

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

    void setZoomFactor(qreal zoomFactor);

    void handleBoatPositionChanged();

private:
    ChartGraphicsObject *m_chartGraphicsObject;
    RouteWidget *routeWidget;
    BoatWidget *boatWidget;
    QGraphicsView *view;

    qreal m_zoomFactor;
    qreal m_zoomSpeed;

    bool m_followBoat;
};

#endif // QTRENDERER_H
