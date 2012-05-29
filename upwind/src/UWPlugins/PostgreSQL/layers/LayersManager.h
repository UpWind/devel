
#ifndef LAYERSMANAGER_H_
#define LAYERSMANAGER_H_


#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QList>
#include <QObject>
#include <QHash>
#include <QtGui>
#include "Layer.h"
#include <QMutex>

#include "LayersLoader.h"
#include "LayersSaver.h"
#include "layersimplificationhandler.h"
#include "../settings/SettingsManager.h"
#include "ChartTableGen.h"
#include "LayerVisualInfo.h"
#include "chart.h"
#include "ChartTable.h"

class ThreadFactory;

class LayersManager: public QObject {
	Q_OBJECT

public:
	LayersManager();
	virtual ~LayersManager();

	static LayersManager* getInstance();

	void setScene(QGraphicsScene* scene);

	void loadLayer(Layer::LayerType type);
	void unLoadLayer(Layer::LayerType type);

	//Layer* getBaseLayer() const;

	void lock();
	void unlock();

	void setZoom(int zoomLevel);
	int getZoom(){return zoomLevel;}

	QHash<Layer::LayerType, Layer*>* getDrawnLayers(){return &drawnLayers;}
	QHash<Layer::LayerType, LayerSimplificationHandler*> *getHandlers(){ return &handlers;}

	void setDrawnLayers( QHash<Layer::LayerType, Layer*> layers ){ drawnLayers = layers; }

	bool isDrawingLines(){return drawingLines;}
	bool isDrawingPoints(){return drawingPoints;}

	void setDrawingLines(bool value){drawingLines = value;}
	void setDrawingPoints(bool value){drawingPoints = value;}

	int getOptimizationStatus();

	void drawPolygonLayers();
	void hidePolygonLayers(); //all private when zoom loading working
	void drawLineLayers();
	void hideLineLayers();
	void drawPointLayers();
	void hidePointLayers();

	void scaleSymbols();
        void scaleSymbols(Layer::LayerType type);
	qreal getScaleFactor();

public slots:
	void optimizeLayers();
	void onLoadingReady(LayerSimplificationHandler* handler);
	void cancelBackgroundOperations();
	void zoomingReady(int zoom);
	void addToScene(Layer* layer);
	void removeFromScene(Layer* layer);

signals:
	void sceneChanged(QString chartName);
	void loadingLayer(QString chartName);
	/**
	 * Emitted when a layer or layer level is loaded OR when zooming is ready.
	 */
	void chartLoaded(QString chartName);

private:

	//void loadOptimizedLayers();

	/**
	 * This mutex protects two things (at least):
	 * load queues and scene.
	 */
	QMutex mutex;
	static LayersManager* instance;
	ThreadFactory* factory;
	LayersSaver* layersSaver;
	QGraphicsScene* scene;

	QHash<Layer::LayerType, Layer*> polygonLayers, lineLayers, pointLayers;
	QHash<Layer::LayerType, Layer*> drawnLayers;
	QHash<Layer::LayerType, LayerSimplificationHandler*> handlers;

	Chart chart;
	ChartTableGen chartTableGen;
	QVector<LayerVisualInfo*> layerVisualInfoVector;
	ChartTable chartTable;
	SettingsManager* settingsManager;
	ChartTableSettings chartTableSettings;

	// queue for priority loading: <layerId, simpLevels>
	QHash<QString, QVector<int> > primaryLoadQueue;
	void removeFromPrimaryLoadQueue(QString layerId);
	// queue for loading layers after priority loading queue is empty: <layerId, simpLevels>
	QHash<QString, QVector<int> > secondaryLoadQueue;
	bool queuedLoadingDebug;

	int zoomLevel;
	bool drawingLines;
	bool drawingPoints;
	
};



#endif /* LAYERSMANAGER_H_ */
