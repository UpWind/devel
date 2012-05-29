#ifndef ROUTEPROVIDERINTERFACE_H
#define ROUTEPROVIDERINTERFACE_H

#include <QString>
#include "../UWPluginInterface/uwplugininterface.h"

class RouteObjectInterface;

class RouteProviderInterface : public UWPluginInterface{
public:
    explicit RouteProviderInterface(){}
    virtual void initialize(QString host, uint port, QString dbName, QString username, QString password) = 0;
    virtual void setAreaFilter(QRectF area) = 0;
    virtual void setZoomLevel(int level) = 0;
    virtual void setChartWidgetSize(QSize size) = 0;
    virtual QVector<RouteObjectInterface*> getRouteObjects() = 0;

private:
    virtual void initConnection() = 0;
};

Q_DECLARE_INTERFACE(RouteProviderInterface,"RouteProviderInterface")

#endif // ROUTEPROVIDERINTERFACE_H
