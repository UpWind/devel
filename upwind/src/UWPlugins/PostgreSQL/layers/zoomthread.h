#ifndef ZOOMTHREAD_H
#define ZOOMTHREAD_H

#include <QThread>
#include <QHash>
#include "Layer.h"
#include <QGraphicsScene>

class LayerSimplificationHandler;
class LayersManager;

class ZoomThread: public QThread
{
    Q_OBJECT
public:
    ZoomThread();
    /** sets the manager, that holds the chartdata*/
    void setData(
               LayersManager* manager);
    void run();

signals:
    /** emitted when zooming is ready */
    void zoomingReady(int zoom);
    /** remove layer from scene */
    void removeLayer(Layer* layer);
    /** add layer to scene */
    void addLayer(Layer* layer);


private:

    void zoom();
    int zoomLevel;
    LayersManager* manager;
};

#endif // ZOOMTHREAD_H
