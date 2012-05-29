#include "threadfactory.h"
#include "LayersLoader.h"
#include "zoomthread.h"
#include "LayersManager.h"

ThreadFactory* ThreadFactory::instance = NULL;

ThreadFactory::ThreadFactory(): zoomingThread(NULL)
{
    layerloaders.clear();
}

ThreadFactory::~ThreadFactory()
{
    QList<Layer::LayerType> keys = layerloaders.keys();
    for(int i=0; i<keys.size();i++)
    {
        LayersLoader* loader = layerloaders.take(keys.at(i));
        loader->terminate();
        delete loader;
    }
    layerloaders.clear();
    if(zoomingThread!=NULL)
    {
        zoomingThread->terminate();
        delete zoomingThread;
        zoomingThread = NULL;
    }

    this->instance = NULL;

}

ThreadFactory* ThreadFactory::getInstance()
{
    if(!ThreadFactory::instance)
    {
        ThreadFactory::instance = new ThreadFactory();
    }
    return ThreadFactory::instance;
}

LayersLoader* ThreadFactory::getLayerLoader(Layer::LayerType type, QVector<LayerVisualInfo*> chartTable, ChartTableSettings* chartTableSettings)
{
    LayersLoader* loader;
    if(layerloaders.contains(type))
    {
        loader = layerloaders.value(type);
    }
    else
    {
		loader = new LayersLoader(chartTable, chartTableSettings);
        loader->setLayerType(type);
        layerloaders.insert(type, loader);
    }

    return loader;
}

bool ThreadFactory::isZoomThreadAvailable()
{
    bool value = true;

    if(zoomingThread!=NULL)
    {
        if(zoomingThread->isRunning())
        {
            value = false;
        }
    }
    return value;
}

ZoomThread* ThreadFactory::getZoomThread(LayersManager* manager)
{
    ZoomThread* returnValue = NULL;
    if(zoomingThread == NULL)
    {
        zoomingThread = new ZoomThread();
        QObject::connect(zoomingThread, SIGNAL(zoomingReady(int)), manager, SLOT(zoomingReady(int)));
        QObject::connect(zoomingThread, SIGNAL(addLayer(Layer*)),manager, SLOT(addToScene(Layer*)));
        QObject::connect(zoomingThread, SIGNAL(removeLayer(Layer*)), manager, SLOT(removeFromScene(Layer*)));
    }
    if(!zoomingThread->isRunning())
    {
        zoomingThread->setData(manager);
        returnValue = zoomingThread;
    }
    return returnValue;

}

void ThreadFactory::terminateAndDeleteAllThreads()
{
    QList<Layer::LayerType> keys  = layerloaders.keys();
    for(int i=0;i<keys.size();i++)
    {
        LayersLoader* loader = layerloaders.take(keys.at(i));
        loader->terminate();
        loader->wait();
        delete loader;
    }
    layerloaders.clear();
    if(zoomingThread!=NULL)
    {
        zoomingThread->terminate();
        zoomingThread->wait();
        delete zoomingThread;
        zoomingThread = NULL;
    }
}
