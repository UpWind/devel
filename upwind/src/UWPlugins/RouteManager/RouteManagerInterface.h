#ifndef ROUTEMANAGERINTERFACE_H
#define ROUTEMANAGERINTERFACE_H

#include <QObject>
#include "../UWPluginInterface/uwplugininterface.h"


class UpWindSceneInterface;
class NMEAInstrumentInterface;

class RouteManagerInterface : public UWPluginInterface
{
public:
    virtual ~RouteManagerInterface() {}
    virtual void connectRouteManager(UpWindSceneInterface* scene, QList<NMEAInstrumentInterface*> instruments) = 0;
    //virtual Route* calculateRoute(QPoint pointA, QPoint pointB ) = 0;



};

Q_DECLARE_INTERFACE(RouteManagerInterface,"RouteManagerInterface")

#endif // ROUTEMANAGERINTERFACE_H
