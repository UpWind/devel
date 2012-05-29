#ifndef CHARTOPTIMIZATIONHANDLER_H
#define CHARTOPTIMIZATIONHANDLER_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QPair>
#include "Layer.h"

class LayerSimplificationHandler : public QObject
{
	Q_OBJECT
public:
	/**
	 * DEPRECATED: For point & light layers
	 */
	LayerSimplificationHandler(QString name,  QHash<QString, Layer*> layers);

	/**
	 * DEPRECATED: For polygon and line layers
	 */
	LayerSimplificationHandler(QString name,  QHash<QString, Layer*> layers,
							   QHash<QPair<int,int>, int> levels, bool simplified);

	/**
	 * For polygon and line layers which are loaded in queues
	 */
	LayerSimplificationHandler(QString layerId, QHash<QPair<int, int>, int> zoomLevels,
							   bool simplified);

	/**
	 * @return name of the chart
	 */
	QString getLayerName();
	virtual ~LayerSimplificationHandler();

	// **
	// * Set zoom levels = chart table converted into this form
	// */
	//void setZoomLevels(QHash<QPair<int, int>, int> levels);

	//void setOptimized(bool opt);

	/**
	 * Add a layer for simplification level.
	 */
	void addLayerLevel(QString layerLevelName, Layer* layer);

	/**
	 * Load the rest of the existing layer levels.
	 */
	void loadUnloadedLayerLevels();

	/**
	 * Set visibility range (minZoom, maxZoom) for a simplification level.
	 */
	void setVisibilityRange(QPair<int, int> zoomRange, int level);

	/**
	 * @return whether layer for the simplification level is loaded.
	 */
	bool isLevelLoaded(int level);
	bool isLevelLoaded(QString layerLevelName);

	void printLayerNames();

public slots:
	/**
	 * @return pointer to a Layer that matches the zoom level
	 */
	Layer* getLayerForZoomLevel(int zoom);

	/**
	 * TODO: Q: what is the modifier?
	 * @return pointer to a Layer that matches the zoom level and new modifier
	 */
	Layer* modifierChanged(int zoom, int modifier);


private:
	/**
	 *finds smallest and biggest zoomlevels defined in zoomLevels
	 */
	void findMinAndMax();

	/**
	 * @return layer that matches the optimizationLevel given
	 */
	Layer* getLayerWithSimplification(int optimizationLevel);

	/**
	 * is this chart optimized?
	 */
	bool optimized;
	QString chartName;

	/**
	 * Layer level name and its data.
	 */
	QHash<QString, Layer*> layers;

	/**
	 * Loaded layer levels.
	 */
	QHash<QString, bool> loadedLayerLevels;

	/**
	 * used to scale zoomlevels
	 */
	float modifier;
	int maxZoom, minZoom, maxOpLevel, minOpLevel;

	/**
	 * zoomlevels. The value of this hash is the optimizationlevel that is
	 * used between the zoomlevels defined in the key pair
	 */
	QHash<QPair<int,int>, int> zoomLevels;
	QPair<int,int> currentLevel;

	bool debug;
};

#endif // CHARTOPTIMIZATIONHANDLER_H
