
#ifndef LAYERSLOADER_H_
#define LAYERSLOADER_H_

#include <QGraphicsPolygonItem>
//#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QBrush>
#include "chart.h"
#include "polygon.h"
#include "Lights/Lighthouse.h"
#include "Lights/Light.h"
#include "Lights/Buoy.h"
#include "Layer.h"
#include <QThread>

#include "../settings/SettingsManager.h"
#include "layersimplificationhandler.h"

#include "LayerVisualInfo.h"
#include "charttablesettings.h"

// QPen STYLES:
// Qt::NoPen          0
// Qt::SolidLine      1
// Qt::DashLine       2
// Qt::DotLine        3
// Qt::DashDotLine    4
// Qt::DashDotDotLine 5
#define STYLE_LIMIT_PEN         2
#define STYLE_LEADINGLNE_PEN    2
#define STYLE_PIPELINE_PEN      4
#define STYLE_SUBMCABLE_PEN     3

#define OPACITY_DEFAULT         1
#define OPACITY_FWAYAREA        0.1
#define OPACITY_BUILTUPARE      0.1
#define OPACITY_BUILDING        0.1
#define OPACITY_SURVQUAL        0.3

#define FONT_FAMILY_SIGNSOUND   "Sans"
#define FONT_SIZE_SIGNSOUND     10
#define FONT_FAMILY_TEXT        "Courier"
#define FONT_SIZE_TEXT          10

#define COLOR_DEFAULT           "NOCOLOR"
#define COLOR_DEPTHCONT         "#3B88D0"
#define COLOR_SALLOW_WATER      "#AAC9E6"
#define COLOR_DEEP_WATER        "#FFFFFF"
#define COLOR_TEXT              "#111111"
#define COLOR_SIGNSOUND         "#22507A"

#define COLOR_GENERAREA_PEN     "#877128"
#define COLOR_GENERAREA_BRUSH   "#FDCA5F"
#define COLOR_LAKE_PEN          "#877128"
#define COLOR_LAKE_BRUSH        "#AAC9E6" // COLOR_SALLOW_WATER
#define COLOR_SPOILGRND_PEN     "#FF00FF"
#define COLOR_LIMIT_PEN         "#bd0f59"
#define COLOR_BUILTUPARE_BRUSH  "#000000"
#define COLOR_FWAYAREA_BRUSH    "#000000"
#define COLOR_SURVQUAL_PEN      "#000000"
#define COLOR_BUILDING_PEN      "#000000"
#define COLOR_BUILDING_BRUSH    "#000000"
#define COLOR_DOCK_PEN          "#000000"
#define COLOR_DOCK_BRUSH        "#000000"
#define COLOR_DEPTHCONT_PEN     "#3B88D0" // COLOR_DEPTHCONT
#define COLOR_COASTLINE_PEN     "#30290F"
#define COLOR_CLOSINGLNE_PEN    "#008000"
//#define COLOR_NAVIGLINE_PEN		"#000000"
#define COLOR_NAVIGLINE_PEN     "#FF0000"
#define COLOR_FERRY_PEN         "#808080"
#define COLOR_HEIGHTCONT_PEN    "#877128"
#define COLOR_LEADINGLNE_PEN    "#000000"
#define COLOR_LOCKNDAM_PEN      "#a0a0a4"
#define COLOR_PIPELINE_PEN      "#bd0f59"
#define COLOR_SUBMCABLE_PEN     "#bd0f59"
#define COLOR_ROADNRAILW_PEN    "#666666"
#define COLOR_BRIDGE_PEN        "#666666"
#define COLOR_TRANSMLINE_PEN    "#000000"
#define COLOR_RIVER_PEN         "#3B88D0"
#define COLOR_SHLNECON_PEN      "#000000"
// OBSTACLES LAYER
#define COLOR_OBSTACLES_PEN		"#000000"
#define COLOR_OBSTACLES_BRUSH	"#000000"
#define OPACITY_OBSTACLES		1
#define ZLEVEL_OBSTACLES		10
#define COLOR_SYMBOLS			"#cc0000"
#define ZLEVEL_SYMBOLS			5.5
//scale factor for symbols
#define SCALE_FACTOR 0.25

class LayersManager;

class LayersLoader: public QThread
{

	Q_OBJECT

public:
	///DO NOT USE THIS METHOD UNLESS YOU ARE SURE WHAT YOU ARE DOING
	static void unlockMutex();

	LayersLoader(QVector<LayerVisualInfo*> layerVisualInfoVector, ChartTableSettings* chartTableSettings);
	~LayersLoader();
	void loadDriver();
	void setLayerType(Layer::LayerType type);

	void run();

	/**
	 * When the loader is run next time, the layer levels from this queue
	 * are loaded.
	 */
	void addToLoadQueue(QString layerId, QVector<int> levels);

	/**
	 * Set this before loading layer. Loader updates the handler, adding the
	 * layer levels from loadQueue into the handler.
	 */
	void setLayerSimplificationHandler(LayerSimplificationHandler * handler);

signals:
	void loadingReady(LayerSimplificationHandler* handler);

private:
	Chart chart;
	Layer::LayerType type;
        QString buildQuery(QString layername, QString layerId = QString() );

	LayerSimplificationHandler* loadLayer(Layer::LayerType type);
	Layer* loadLayerLevel(QString layerId, int level);

        QHash< QString, QList<Polygon> > getPolygonVector(QString layername);

	/**
	 * Load polygons for a single simplification level of a single layer (line or polygon layer).
	 * In practice load polygons from a single source table.
	 * @param layerId layer id (name)
	 * @param srcTableName name of the source DB table
	 * @return polygons from source table
	 */
        QList<Polygon> loadPolygonsFromTable(QString layerId, QString srcTableName);

	// load all simplification levels of layer
	// DEPRECATED

	/*LayerSimplificationHandler* loadPolygonLayerHandler(QString layername, qreal r,
                                                      QString pen_color=COLOR_DEFAULT,
                                                      QString brush_color=COLOR_DEFAULT,
                                                      float opacity=OPACITY_DEFAULT);
	LayerSimplificationHandler* loadLineLayerHandler( QString layername, qreal r,
                                                    QString pen_color=COLOR_DEFAULT,
                                                    int pen_style=1);
	LayerSimplificationHandler* loadDepthareaLayerHandler();
	LayerSimplificationHandler* loadObstaclesLayer();
	LayerSimplificationHandler* loadLightsLayerHandler();
	LayerSimplificationHandler* loadPointLayerHandler(QString symbol, QString layername);
	LayerSimplificationHandler* loadWreckLayerHandler();
	LayerSimplificationHandler* loadSignSoundLayerHandler();
	LayerSimplificationHandler* loadTextLayerHandler();
	LayerSimplificationHandler* loadRocksLayerHandler();*/

	// Load a single simplification level of a layer.
	// This is used for queued loading of layer levels.

	Layer* loadPolygonLayerLevel(QString layerId, int level, qreal zValue,
                                      QString pen_color=COLOR_DEFAULT,
                                      QString brush_color=COLOR_DEFAULT,
                                      float opacity=OPACITY_DEFAULT);
	Layer* loadDepthAreaLayerLevel(int level);
	Layer* loadLineLayerLevel(QString layerId, int level, qreal r,
							  QString pen_color=COLOR_DEFAULT, int pen_style=1);
	Layer* loadObstaclesLayerLevel();
	Layer* loadLightsLayerLevel();
	Layer* loadPointLayerLevel(QString symbol, QString layername);
	Layer* loadWreckLayerLevel();
	Layer* loadSignSoundLayerLevel();
	Layer* loadTextLayerLevel();
	Layer* loadRocksLayerLevel();
        Layer* loadNavaId();

	SettingsManager* settingsManager;
	ChartTableSettings* chartTableSettings;

	QHash<QPair<int,int>,int> getVisibilityRanges(QString layername);
	QVector<LayerVisualInfo*> layerVisualInfoVector;

	QString getSrcTableForSimpLevel(QString layerId, int level);
	
	QHash<QString, QVector<int> > loadQueue;
	//QMutex loadQueueMutex;
	void removeFromLoadQueue(QString layerId, int level);

	LayerSimplificationHandler * queuedHandler;
	bool debug;	

        QString ntypnavaidToString(int value);
        //void setOffset(QGraphicsPixmapItem& item);
};

#endif /* LAYERSLOADER_H_ */
