#ifndef CHARTTABLE_H
#define CHARTTABLE_H

#include <QVector>
#include "LayerVisualInfo.h"

/**
 * Class to contain QVector<LayerVisualInfo*> and to do helpful things with it.
 * Use ChartTableGen class to actually generate the mentioned vector.
 */
class ChartTable
{
public:
	ChartTable();
	~ChartTable();

	void setLayerVisualInfoVector(QVector<LayerVisualInfo*> lviVector);
	QVector<LayerVisualInfo*> getLayerVisualInfoVector();

	/**
	 * @return Number of simplification levels for layer.
	 */
	int getSimpLevelCount(QString layerId);

	/**
	 * @return Layer level name.
	 */
	QString getLayerLevelName(QString layerId, int level);

	/**
	 * @return simplification level for zoom level or -1 if not found
	 */
	int getSimpLevelForZoom(QString levelId, int zoomLevel);

	/**
	 * @return level numbers of layer.
	 */
	QVector<int> getLevels(QString layerId);

private:
	QVector<LayerVisualInfo*> layerVisualInfoVector;
};

#endif // CHARTTABLE_H
