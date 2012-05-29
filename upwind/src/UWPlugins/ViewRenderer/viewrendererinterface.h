#ifndef VIEWRENDERERINTERFACE_H
#define VIEWRENDERERINTERFACE_H

#include <QObject>
#include <QtGui/QtGui>

#include "../UWPluginInterface/uwplugininterface.h"
#include "../ChartProviderInterface/corechartprovider.h"

class UpWindSceneInterface;

class ViewRendererInterface : public UWPluginInterface
{
public:
    virtual ~ViewRendererInterface(){}

    virtual void ConnectPlugin(UpWindSceneInterface* scene, QWidget* frame, CoreChartProvider* model) = 0;
    virtual void paint() = 0;
};

Q_DECLARE_INTERFACE(ViewRendererInterface,"ViewRendererInterface")

#endif // VIEWRENDERERINTERFACE_H
