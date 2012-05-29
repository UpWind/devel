#ifndef COREROUTEMANAGER_H
#define COREROUTEMANAGER_H

#include <QObject>
#include <QLayout>
#include "RouteManagerInterface.h"
#include "../UWPluginInterface/uwplugininterface.h"
#include "../../Settings/settings.h"
#include "../UpWindScene/UpWindSceneInterface.h"
#include "../NMEAInstruments/CoreNMEAInstrument/nmeainstrumentinterface.h"


class CoreRouteManager: public QObject, public RouteManagerInterface
{
    Q_OBJECT
    Q_INTERFACES(RouteManagerInterface UWPluginInterface)


public:
    CoreRouteManager();
    /**
      * Adds the plugin to the given layout. The plugin will no longer be on it's own window.
      * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
      */
    void addPluginSettingsToLayout(QLayout *layout) ;

    /**
      * Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    QString getName() ;

    Settings* getSettings();

    void connectRouteManager(UpWindSceneInterface* scene, QList<NMEAInstrumentInterface*> instruments);

    //This should calculate and return the route between pointA and pointB
    Route* calculateRoute(QPointF pointA, QPointF pointB );

    /**
    * Creates a bunch of points in a vector or similar that connect the point A and point B
    *
    *
    **/
    //void createRoute()


    /**
      *Calculates the actual path where obstacles are avoided.
      *
      *
      **/
    //void calculateActualRoute()

private:
    Settings * settings;
    QList<NMEAInstrumentInterface*> instruments;
    UpWindSceneInterface* scene;


protected slots:

signals:

};

#endif // COREROUTEMANAGER_H
