#ifndef CHART_H
#define CHART_H

#include "ogrsf_frmts.h"
#include "ogr_api.h"
#include "ogr_srs_api.h"
#include "ogr_geometry.h"
#include "cpl_error.h"
#include "cpl_string.h"

#include <QDir>
#include <QTextStream> // needed for loading the .VDF files
#include <QList>
#include <QVector>
#include <QHash>
#include <iostream>
#include <QDebug>
//#include "../UserInterface/BadFileFormatDialog.h"
#include "polygon.h"
#include "line.h"
#include "point.h"
#include "../settings/SettingsManager.h"
#include "../shared/uwmath.h"

#define DEBUG 1

class QDir;
class QPolygonF;
class OGRDataSource;
class OGRLayer;

class Chart {
public:
        Chart(/*QString driver*/);
        virtual ~Chart();

        /**
         * Set driver for OGR data source.
         * @param driver string in format defined by OGR
         */
        void setDriver(QString driver);

        /**
         * Does the same as setDriver() but requires only DB driver name in Qt format.
         * @param qSqlDriver Database driver string as defined by Qt
         */
        void setOGRDriverQt(QString qSqlDriver);

        // NOTE: when using the chart class with database
        // 		 the layerName parameter needs to contain valid SQL statement
        /**
         * Get a list of polygons of the specified layer using the database and the OGR libraries.
         * @param QString layerName name of the layer.
         * @param queryStr SQL query with which to query the layer data from database. Usually \"SELECT * FROM layername\"
         * @return QList<Polygon> List of polygons.
         */
        QList<Polygon> getChartPolygons(QString layerName, QString queryStr);

        /**
         * Load chart lines as open Polygons. In early versions of this software these used to be Lines,
         * but it was unusably slow.
         * @param layerName Layer name.
         * @param queryStr SQL query string with which to query data. Usually "SELECT * FROM <layerName>".
         */
        QList<Polygon> getChartLines(QString layerName, QString queryStr);
        QList<Point> getChartPoints(QString layerName, QString queryStr);


        QList<Point> getChartPointsArea(QString lay, double x, double y,double i,double j);
        QList<Line> getChartLinesArea(QString layerName,double x,double y,double i,double j);
        QList<Polygon> getChartPolygonsArea(QString layerName,double x,double y,double i,double j);

        /**
         * For saving and simplification. Gets all the layernames in DB and returns them
         * inside a vector.
         * @return QVector<QString*> Returns vector with layernames
         */
        QVector<QString*> getLayersNames();

        /**
         * Get base table names from DB for layers which are to be simplified:
         * line and polygon layers.
         */
        QStringList getSimplifiedLayers();

        /**
         * Get base table names from DB for layers which are not to be simplified:
         * point layers.
         */
        QStringList getNonSimplifiedLayers();

        /**
         * Creates a copy layer of original layer and returns a pointer to it. Copy layer can already exist,
         * when new copy overwrites the old one.
         * @param  srcLayer Name of the source layer, which is about to be copied
         * @param  copyLayerName Name for a copy layer
         * @return OGRLayer* Pointer to copy layer
         */
        OGRLayer* getLayerCopy( QString srcLayer, QString copyLayerName );
        OGRLayer *getLayerFromDatabase(QString sqlQuery);
        QList<QString> getFieldsForLayer(const QString &layerName);

private:


        OGRLayer *getLayerFromSHPFile(QString fileName);

        QDir createDirectory(QString path);

        /** copies made from the original datasource (needed for multithreading the optimization) */

        QHash<QString, QList<QString> > fieldsForLayers;
        OGRDataSource *dataSource;

        // layerInfo is either SQL query or the layer's name
        typedef OGRLayer *(Chart::*GetLayer)(QString layerInfo);
        GetLayer getLayer;

        SettingsManager *settingsManager;
        bool debug;
};
#endif /* CHART_H_ */
