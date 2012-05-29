#ifndef THREADFACTORY_H
#define THREADFACTORY_H

#include "Layer.h"
#include <QHash>
#include <QObject>
#include "LayerVisualInfo.h"
#include "charttablesettings.h"

class ZoomThread;
class LayersLoader;
class LayerSimplificationHandler;
class LayersManager;

/**
 * Singleton class designed to keep track of the threads used
 */
class ThreadFactory: public QObject
{
    Q_OBJECT
public:
	/**
	 * return layersloader instance
	 */
	LayersLoader* getLayerLoader(Layer::LayerType type, QVector<LayerVisualInfo*> chartTable, ChartTableSettings* chartTableSettings);

    ZoomThread* getZoomThread(LayersManager* manager);
	/**
	 * Returns instance of this factory
	 */
    static ThreadFactory* getInstance();

	/**
	 * Zoom thread is available if it is not already doing zooming operations
	 */
    bool isZoomThreadAvailable();

	/**
	 * Stops and deletes all threads. Use carefully.
	 */
    void terminateAndDeleteAllThreads();

    ~ThreadFactory();
private:
    /** static instance of this class */
    static ThreadFactory* instance;
    /** layerloader classes instansiated */
	QHash<Layer::LayerType, LayersLoader*> layerloaders;
    /** zooming thread (at the moment, only one thread can be alive) */
    ZoomThread* zoomingThread;

    ThreadFactory();

};



#endif // THREADFACTORY_H
