/**
 * @file LayerVisualInfo.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef LAYER_VISUAL_INFO_H_
#define LAYER_VISUAL_INFO_H_

#include <QString>

/**
 * LayerVisualInfo contains information about the Layer's visibility range as
 * maxZoom and minZoom, from which table to get the data (source table), graphical style id,
 * simplification settings: number of levels and simplification degree of
 * each level (epsilon value).
 *
 * Use ChartTableGen class to easily generate needed LayerVisualInfo objects.
 *
 */
class LayerVisualInfo {
public:

	LayerVisualInfo();
	LayerVisualInfo( const LayerVisualInfo &);

	QString getLayerId();

	/**
	 * Minimum zoom level for the zoom range within which the layer is visible.
	 */
	int getZoomMin();

	/**
	 * Maximum zoom level for the zoom range within which the layer is visible.
	 */
	int getZoomMax();

	/**
	 * Style id for the current zoom range.
	 */
	int getStyleId();

	/**
	 * Get simplification level.
	 * Level = 0: no simplification. Level > 0: simplified.
	 */
	int getSimpLevel();

	/**
	 * Get the used epsilon value.
	 */
	double getEpsilon();

	/**
	 * The name of the table which contains the layer data.
	 * This can be used to select the layer data containing the simplified
	 * data for the current zoom range.
	 */
	QString getSrcTableName();

	void setLayerId(QString layerId);
	void setZoomMin(int zoomMin);
	void setZoomMax(int zoomMax);
	void setStyleId(int styleId);
	void setSimpLevel(int simpLevel);
	void setEpsilon(double epsilon);
	void setSrcTableName(QString srcTableName);

	QString toString();

private:
	QString layerId;
	int zoomMin;
	int zoomMax;
	int styleId;
	int simpLevel;
	double epsilon;
	QString srcTableName;
};

#endif // LAYER_VISUAL_INFO_H_
