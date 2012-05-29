#include "coreroutemanager.h"

#include <QtGui>


CoreRouteManager::CoreRouteManager()
{
    settings = 0;
}

QString CoreRouteManager::getName(){
    return QString("CoreRouteManager");
}
void CoreRouteManager::addPluginSettingsToLayout(QLayout *layout) {}

Settings * CoreRouteManager::getSettings(){
    return settings;
}

Route* CoreRouteManager::calculateRoute(QPointF pointA, QPointF pointB ){
    return 0;
}

void CoreRouteManager::connectRouteManager(UpWindSceneInterface *scene, QList<NMEAInstrumentInterface *> instruments){
    /* All of the data that is needed for route managing should be in
     * UpWindScene and the instruments..
     *
     * The route should be stored in the UpWindScene, and the renderer can then render it.
     */
    this->scene = scene;
    this->instruments = instruments;
}


Q_EXPORT_PLUGIN2(coreroutemanager, CoreRouteManager);
