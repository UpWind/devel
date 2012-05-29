/**
 * @file ChartTable.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef CHARTTABLEGEN_H
#define CHARTTABLEGEN_H

//#include <QSqlDatabase>
//#include <QSqlQuery>
//#include <QSqlError>
#include <QList>
#include <QListIterator>
#include <QVector>
#include <QVectorIterator>
#include "math.h"
//#include <QtGui>
#include <QFile>
//#include <QString>
#include <QtXml>
#include "../settings/SettingsManager.h"
#include "LayerVisualInfo.h"
#include "charttablesettings.h"
#include <limits.h>

/**
 * Load and save chart tables from database or XML file, or generate a new table.
 * Chart table contains visual information about the layers (LayerVisualInfo class).
 *
 * LayerVisualInfo contains information about layer's visibility range, style id,
 * number of existing simplification levels, simplification degree (epsilon value)
 * and source tables. This information can then be used by LayersManager to toggle
 * layer visibility.
 *
 * You have to set generator parameters either using setSettings() method or
 * setting the parameters individually with the following methods.
 * <ul>
 * <li>setOriginalZoomLevel()
 * <li>setScaleFactor()
 * <li>setSimpLevelCount()
 * <li>setMinZoomLevel()
 * <li>setMaxZoomLevel()
 * <li>setMinEpsilon()
 * <li>setMaxEpsilon()
 * </ul>
 *
 * Then you can use either generateLayerVisualInfoForAllTables() to generate all layer
 * data at once, or generateLayerVisualInfoForLayer() for a single layer.
 *
 * This class uses SettingsManager for database and XML file settings.
 *
 * @author UpWind::Performance 2009 / Henrik Pitkala
 * @date 2009-10-28 (started)
 */
class ChartTableGen {

public:

	ChartTableGen();
	~ChartTableGen();

	/**
	 * Set the original zoom level of the map. This is the value of zoomLevel when
	 * the map has not been zoomed in or out.
	 */
	void setOriginalZoomLevel(double originalZoomLevel);

	/**
	 * Get the original zoom level of the map. This is the value of zoomLevel when
	 * the map has not been zoomed in or out.
	 */
	double getOriginalZoomLevel();

	/**
	 * Set the scale factor when zooming in/out the map. The original zoom level
	 * is multiplied by this value when zooming in and divided by this value
	 * when zooming out.
	 */
	void setScaleFactor(double scaleFactor);

	/**
	 * Set the scale factor when zooming in/out the map. The original zoom level
	 * is multiplied by this value when zooming in and divided by this value
	 * when zooming out.
	 */
	double getScaleFactor();

	/**
	 * Set the count of simplification levels. The actual data set count will be
	 * count + 1 because there will still be the the original data set
	 * for level 0.
	 */
	void setSimpLevelCount(int count);

	/**
	 * Get simplification level count. The level count tells how many simplified
	 * levels there are. The actual count of the data sets will be count + 1
	 * as there exists the original data set.
	 */
	int getSimpLevelCount();

	/**
	 * Calculate the epsilon value for simplification level.
	 */
	double getEpsilonForLevel(int simpLevel);

	/**
	 * Calculate the maximum zoom level for simplification level.
	 * Please note that for simplification levels 0 and getSimpLevelCount()
	 * require manual value setting.
	 */
	double getZoomForLevel(int simpLevel);

	/**
	 * Zoom levels: minZoomLevel = minimum zoom level value when zoomed out,
	 * maxZoomLevel = maximum zoom level value when zoomed in.
	 */
	void setMinZoomLevel(double minZoomLevel);

	/**
	 * Set the maximum zoom level value (when zoomed in).
	 */
	void setMaxZoomLevel(double maxZoomLevel);

	/**
	 * Set minimum epsilon = the smallest amount of simplification.
	 */
	void setMinEpsilon(double minEpsilon);

	/**
	 * Set maximum epsilon = the largest amount of simplification.
	 */
	void setMaxEpsilon(double maxEpsilon);

	/**
	 * Get the minimum zoom level (when zoomed out the map).
	 */
	double getMinZoomLevel();

	/**
	 * Get the maximum zoom level (when zoomed in the map).
	 */
	double getMaxZoomLevel();

	/**
	 * Get the minimum epsilon.
	 */
	double getMinEpsilon();

	/**
	 * Get the maximum epsilon.
	 */
	double getMaxEpsilon();

	void setSettings(ChartTableSettings settings);
	ChartTableSettings getSettings();

	/**
	 * Generate a QVector which contains LayerVisualInfo for the specified
	 * layerId.
	 *
	 * @param ok After successful execution this boolean is set to true, otherwise it is set to false.
	 */
	QVector<LayerVisualInfo*> generateLayerVisualInfoForLayer(QString layerId);

	/**
	 * Load all polygon and line layers from the DB and generate a QVector which
	 * contains LayerVisualInfo objects for all of them. Polygon and line layers
	 * are those tables in the DB which end with "_r" or "_l" (bad coding :( TODO: fix this).
	 *
	 * @param layerList List of layers.
	 * @param ok After successful execution this is set to true, otherwise it is set to false.
	 */
	QVector<LayerVisualInfo*> generateLayerVisualInfoForAllTables(QStringList layerList, bool *ok);


	/* DEPRECATED
	 *
	 * Save the layer visual info into the specified table in the database.
	 * All old data is cleared from the table if it exists.
	 *
	 * @param tableName The name of the table into which the data is saved. If empty name, the name is taken from SettingManager. If still empty, don't do anything. Table is created if it doesn't exist.
	 * @return true: success, false: error
	 */
	//bool saveChartTableToDB(QVector<LayerVisualInfo*> data, QString tableName="");

	/* DEPRECATED
	 *
	 * Load the layer visual info from the database.
	 * @param ok After successful execution this is set to true, otherwise it is set to false.
	 * @return LayerVisualInfo objects which are found from the database. If there is some error, the vector will be empty.
	 */
	//QVector<LayerVisualInfo*> loadChartTableFromDB(QString tableName, bool *ok);

	/**
	 * Save the layer visual info into the specified file. The file format is XML.
	 * The existing data is overwritten.
	 *
	 * @param fileName The name of the file into which the data is saved. If empty name, the name is taken from SettingManager. If still empty, don't do anything. Table is created if it doesn't exist.
	 * @return true: success, false: error
	 */
	bool saveChartTableToXML(QVector<LayerVisualInfo*> data, QString fileName="");

	/**
	 * Load the layer visual info from XML file.
	 * @param ok After successful execution this is set to true, otherwise it is set to false.
	 * @return LayerVisualInfo objects which are found from the file. If there is some error, the vector will be empty and ok will be false.
	 */
	QVector<LayerVisualInfo*> loadChartTableFromXML(QString fileName, bool *ok);

private:
	//SettingsManager* settingsManager;
	//QSqlDatabase db;
	double originalZoomLevel;
	double scaleFactor;
	int simpLevelCount;
	double minZoomLevel;
	double maxZoomLevel;
	double minEpsilon;
	double minEpsilonPow;	// power of 10 for minEpsilon
	double maxEpsilon;
	double maxEpsilonPow;	// power of 10 for maxEpsilon

	ChartTableSettings settings;

	/**
	 * Create the root start and end elements into the XML file. If the file exists,
	 * it is cleaned. If the file doesn't exist, it is created.
	 * Root element is <charttable>.
	 */
	void createXML();

	/**
	 * Get the root element <charttable> of the XML file.
	 */
	QDomElement getXMLRoot();
	QFile *XMLFile;
	QXmlStreamWriter *XMLWriter;
	QDomDocument XMLDoc;

};

#endif // CHARTTABLEGEN_H
