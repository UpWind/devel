#ifndef CHARTPROVIDERINTERFACE_H
#define CHARTPROVIDERINTERFACE_H

#include <QString>
#include "../UWPluginInterface/uwplugininterface.h"

class ChartObjectInterface;

class ChartProviderInterface : public UWPluginInterface{
public:
    explicit ChartProviderInterface(){}
    virtual void initialize(QString host, uint port, QString dbName, QString username, QString password) = 0;
    virtual void setAreaFilter(QRectF area) = 0;
    virtual void setZoomLevel(int level) = 0;
    virtual void setChartWidgetSize(QSize size) = 0;
    virtual QVector<ChartObjectInterface*> getChartObjects() = 0;
    virtual QRectF getChartBoundaries() = 0;
    virtual QSet<QString> getSelectedLayers() = 0;

private:
    virtual void initConnection() = 0;
};

Q_DECLARE_INTERFACE(ChartProviderInterface,"ChartProviderInterface")

#endif // CHARTPROVIDERINTERFACE_H
