#ifndef COREVIEWRENDERER_H
#define COREVIEWRENDERER_H

#include <QObject>
#include "../../Settings/settings.h"

#include "viewrendererinterface.h"
#include "../ChartProviderInterface/corechartprovider.h"

class UWCore;

class CoreViewRenderer: public QObject, public ViewRendererInterface
{
    Q_OBJECT
    Q_INTERFACES(ViewRendererInterface UWPluginInterface)

public:
    CoreViewRenderer();

    void addPluginSettingsToLayout(QLayout *layout) ;

    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    QString getName();

    Settings* getSettings();

    void paint();

    void ConnectPlugin(UpWindSceneInterface* scene, QWidget* frame, CoreChartProvider* model);

protected:
    UpWindSceneInterface* scene;
    QWidget* frame;
    ChartProviderInterface *model;
    Settings* settings;

protected slots:
    void zoomIn();
    void zoomOut();
    void expand();
    void zoomBoat();
    void rotateLeft();
    void rotateRight();
    void zoomToolActivated(bool);
    void simModeChanged(bool);
    void drawRoute(bool);
};

#endif // COREVIEWRENDERER_H
