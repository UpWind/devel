#ifndef CORECHARTPROVIDER_H
#define CORECHARTPROVIDER_H

#include <QObject>
#include "chartproviderinterface.h"

class CoreChartProvider : public QObject, public ChartProviderInterface
{
    Q_OBJECT
    Q_INTERFACES(ChartProviderInterface UWPluginInterface)

public:
    explicit CoreChartProvider() {}

signals:
    void modelChanged();
};

#endif // CORECHARTPROVIDER_H
