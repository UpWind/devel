#ifndef COREROUTEPROVIDER_H
#define COREROUTEPROVIDER_H

#include <QObject>
#include "routeproviderinterface.h"

class CoreRouteProvider : public QObject, public RouteProviderInterface
{
    Q_OBJECT
    Q_INTERFACES(RouteProviderInterface UWPluginInterface)

public:
    explicit CoreRouteProvider(QObject *parent = 0){}

signals:
    void modelChanged();
};

#endif // COREROUTEPROVIDER_H
