#include "route.h"

Route::Route(QObject *parent) :
    QObject(parent)
{
}


void Route::setRoutePoints(QList<QPoint *> list){

}

QList<QPoint*> Route::getRoutePoints(){
    return this->routePoints;
}
