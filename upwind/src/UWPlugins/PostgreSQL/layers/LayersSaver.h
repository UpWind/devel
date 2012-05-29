#ifndef LAYERSSAVER_H_
#define LAYERSSAVER_H_

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
#include <QObject>
#include <QVector>
#include <QThread>

#include "../settings/SettingsManager.h"
#include "ogroptimizer.h"

//#include "ChartTableGen.h"
#include "charttablesettings.h"
#include "LayerVisualInfo.h"

class LayersSaver: public QThread
{
    Q_OBJECT

public:
		LayersSaver(QVector<LayerVisualInfo*> chartTable, ChartTableSettings* chartTableSettings);
        ~LayersSaver();
        void loadDriver();
        int getOptimizationStatus();
        SettingsManager * getSettingsManager();
        void setup();
        void run();

public slots:
        /**
          * Runs through every line and region layer in the DB, makes copy of them (levels)
          * and optimizes the copy ones
          */
        void saveLayers();

        void optimizerReady();


private:
        int totalNumberOfLayers;
        int layersOptimized;
        QVector<OGROptimizer*> optimizers;

        Chart chart;
        SettingsManager* settingsManager;

        void optimize(OGRLayer* layer, OGRLayer* original_layer, qreal level);
        void altOptimize( QString & layerId, QString &query, qreal level);
        // Line = l, region = r
        QString selectedGeomTypes;

        // How many optimization levels are made
        int optimLevels;

        // Appends the "_level" string in to optimized tablenames
        QString appendLevelStr;

        /**
          * Returns 1 if layerName is correct in terms of selectedGeomTypes ( l=line, r=region ),
          * and the second last is "_"
          * @param QString layerName
          * @return 1 or 0
          */
        int checkGeometryType( QString layerName );

        qreal getEpsilon( QString layername );

        /* For ChartTable and layersVisualInfo */
		//ChartTableGen* chartTable;
		ChartTableSettings* chartTableSettings;
		QVector<LayerVisualInfo*> chartTable;
        void loadChartTable();
        QMutex mutex;

};

#endif // LAYERSSAVER_H
