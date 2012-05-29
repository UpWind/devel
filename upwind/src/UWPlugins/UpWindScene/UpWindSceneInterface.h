#ifndef UPWINDSCENEINTERFACE_H
#define UPWINDSCENEINTERFACE_H

#include <QObject>

#include "../UWPluginInterface/uwplugininterface.h"
#include "Scene/boat.h"
#include "../ChartProviderInterface/corechartprovider.h"


class Route;

class UpWindSceneInterface : public UWPluginInterface
{
public:

    virtual  Boat* getBoat() = 0;
    virtual  Route* getRoute() = 0;


    /** Parse the NMEA string if it is a GPS NMEA string
      * @param text - NMEA string
      */
    virtual void parseNMEAString( const QString & text) = 0;

    virtual void setChartObjects(CoreChartProvider *model) = 0;
    virtual void setBoat(Boat *boat) = 0;

    virtual ~UpWindSceneInterface(){}
};

Q_DECLARE_INTERFACE(UpWindSceneInterface,"UpWindSceneInterface")

#endif // UPWINDSCENEINTERFACE_H
