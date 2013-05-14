//#define QT_NO_DEBUG_OUTPUT

#include "postgrechartprovider.h"
#include "uwmath.h"
#include "coastline.h"
#include "deptharea.h"
#include "depthareashallow.h"
#include "limit.h"
#include "rock.h"
#include "wreck.h"
#include "navigline.h"
#include "transmline.h"
#include "bridge.h"
#include "deptharea.h"
#include "depthcontour.h"
#include "navaid.h"
#include "generarea.h"
#include "signsound.h"

#include "ogrsf_frmts.h"
#include <QDebug>
#include <QString>
#include <QSet>
#include <QVector>

PostgreChartProvider::PostgreChartProvider() :
    dataSource(0),
    detailLevel(0),
    settings(0)
{
    selectedLayers.insert("deptharea_r");
    selectedLayers.insert("generarea_r");
    selectedLayers.insert("signsound_p");
    selectedLayers.insert("depthcont_l");
    selectedLayers.insert("navaid_p");
    selectedLayers.insert("navigline_l");
    selectedLayers.insert("rock_p");
    selectedLayers.insert("wreck_p");
    selectedLayers.insert("transmline_l");
    // selectedLayers.insert("bridge_l");

    initializeSettings();
    settingsUI = new SettingsUI();
    settingsUI->setPGChart(this);
    settingsUI->setupSettings(settings);
}

PostgreChartProvider::~PostgreChartProvider(){
    OGRDataSource::DestroyDataSource(dataSource);
    foreach(ChartObjectInterface *object, chartObjects)
        delete object;
}

void PostgreChartProvider::initialize(QString host,
                                      uint port,
                                      QString dbname,
                                      QString username,
                                      QString password,
                                      int detailLevel)
{
    dbHost = host;
    dbPort = port;
    dbName = dbname;
    dbUser = username;
    dbPass = password;
    this->detailLevel = detailLevel;
    initConnection();
}

void PostgreChartProvider::initConnection(){
    OGRRegisterAll();
    QString driver = QString("PG:dbname=" + dbName +
                             " user=" + dbUser +
                             " password=" + dbPass +
                             " port=" + QString::number(dbPort) +
                             " host=" + dbHost);
    dataSource = OGRSFDriverRegistrar::Open(driver.toAscii(), FALSE);

    if(dataSource == NULL){
        qWarning() << "OGR Error: Failed to open database connection";
        //we should warn the user that it must set up the database connection
    }
    else {
        mapLayers();
        if (detailLevel == 0)
            setBoundingBox(layers.value("generarea_r"));
        else
            setBoundingBox(layers.value("generarea_r_level" + QString::number(detailLevel)));
    }
}

QSet<QString> PostgreChartProvider::getSelectedLayers()
{
    return selectedLayers;
}

void PostgreChartProvider::mapLayers()
{
    OGRLayer *layer;

    foreach(QString baseName, selectedLayers)
    {
        // Get all the 6 layerlevels.
        /*
        for (int i = 1; i < 7; i++)
        {
            layer = getLayerLevel(baseName, i);

            // If the layer was found from the database.
            if (layer != NULL)
            {
                QString name = QString::fromLocal8Bit(layer->GetLayerDefn()->GetName());
                layers.insert(name, layer);
            }
        }
        */

        // Get the correct level.
        layer = getLayerLevel(baseName, detailLevel);

        // If the layer was found from the database.
        if (layer != NULL)
        {
            QString name = QString::fromUtf8(layer->GetLayerDefn()->GetName());
            layers.insert(name, layer);
        }
    }
}

// Gets layer by basename of wanted layer-level.
OGRLayer* PostgreChartProvider::getLayerLevel(QString layerNameString, int layerNumber)
{
    qDebug() << "GetLayerLevel" << layerNumber;
    OGRLayer *layer;
    // Layer's levelnumber to QString
    QString layerNumberStr = QString::number(layerNumber);

    if(layerNameString.at(layerNameString.size()-1) != 'p' && layerNumber != 0)
        layerNameString = layerNameString + "_level" + layerNumberStr;
    else
        layerNameString = layerNameString;

    // Convert to const char*
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray encodedString = codec->fromUnicode(layerNameString);
    const char* layerName = encodedString.constData();

    layer = dataSource->GetLayerByName(layerName);

    return layer;
}

/*
void PostgreChartProvider::mapLayers(){
    OGRLayer *layer;
    QString layerName, baseName;
    int layerCount = dataSource->GetLayerCount();

    for(int i = 0; i < layerCount; ++i) {
        layer = dataSource->GetLayer(i);
        layerName = layer->GetLayerDefn()->GetName();

        baseName = cleanupLayerName(layerName);

        if(selectedLayers.contains(baseName))
            layers.insert(layerName, layer);

    }      
}
*/

void PostgreChartProvider::setBoundingBox(OGRLayer *layer){
    OGRGeometry *geometry;
    OGRFeature *feature;
    OGRMultiPolygon *multiPoly;
    OGRPolygon *poly;
    OGREnvelope *envelope;
    int polyCount;
    double minX = 180, minY = 90, maxX = -180, maxY = -90;

    envelope = new OGREnvelope();
    layer->ResetReading();

    while((feature = layer->GetNextFeature()) != NULL){
        geometry = feature->GetGeometryRef();

        if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbPolygon){
            poly = (OGRPolygon *) geometry;

            poly->getEnvelope(envelope);
            minX = qMin(minX, envelope->MinX);
            minY = qMin(minY, envelope->MinY);
            maxX = qMax(maxX, envelope->MaxX);
            maxY = qMax(maxY, envelope->MaxY);
        }
        else if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon){
            multiPoly = (OGRMultiPolygon *) geometry;

            polyCount = multiPoly->getNumGeometries();

            for(int i = 0; i < polyCount; ++i){
                poly = (OGRPolygon *) multiPoly->getGeometryRef(i);

                poly->getEnvelope(envelope);
                minX = qMin(minX, envelope->MinX);
                minY = qMin(minY, envelope->MinY);
                maxX = qMax(maxX, envelope->MaxX);
                maxY = qMax(maxY, envelope->MaxY);
            }
        }
        else
            qDebug() << Q_FUNC_INFO << ": No polygons geometry";
        OGRFeature::DestroyFeature(feature);
    }
    chartBoundaries = QRectF(UwMath::toConformalInverted(QPointF(minX, maxY)),
                             UwMath::toConformalInverted(QPointF(maxX, minY)));
}

void PostgreChartProvider::setAreaFilter(QRectF area) {
    filter = area;

    QHash<QString, QList<ChartObjectInterface::ChartElement> > chartPointLayers;

    foreach (OGRLayer *layer, layers) {
        OGRwkbGeometryType layerType = wkbFlatten(layer->GetLayerDefn()->GetGeomType());
        const QString &layerName = layer->GetLayerDefn()->GetName();

        if (layerType == wkbPoint || layerType == wkbMultiPoint) {
            chartPointLayers.insert(layerName, getPoints(layer));

        } else if((detailLevel == 0 && !layerName.contains("level"))
             || (detailLevel != 0 && layerName.contains("level" + QString::number(detailLevel)))) {

            if(layerType == wkbPolygon || layerType == wkbMultiPolygon)
                getPolygons(layer);
            else if(layerType == wkbLineString || layerType == wkbMultiLineString)
                getLines(layer);

        }
    }

    //TODO Add all selected layers as ChartObject to chartObjects vector
    foreach (const QString layerName, layersPixelGeometry.keys()) {
        ChartObjectInterface *obj = NULL;
        const QString &baseName = cleanupLayerName(layerName);

        if (baseName == "deptharea_r") {

            getDepths(layers.value(layerName));

            QVector<QPolygonF> shallow;
            QVector<QPolygonF> deep;
            QVector<QPolygonF> shallowgeo;
            QVector<QPolygonF> deepgeo;
            QVector<QPolygonF> wkb = layersPixelGeometry.value(layerName);
            QVector<QPolygonF> wkb2 = layersCoordinateGeometry.value(layerName);

            for (int l = 0; l < depths.size(); l++) {
                if (depths.at(l) >= 10) {
                    deep.append(wkb.at(l));
                    deepgeo.append(wkb2.at(l));
                } else {
                    shallow.append(wkb.at(l));
                    shallowgeo.append(wkb2.at(l));
                }
            }

           obj = new DepthArea(deep,deepgeo,layers.value(layerName), baseName);
           chartObjects.append(obj);
           obj = new DepthAreaShallow(shallow,shallowgeo,layers.value(layerName), baseName);

       //    obj = new DepthArea(layersPixelGeometry.value(layerName),layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        } else if(baseName == "generarea_r") {
           obj = new CoastLine(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        } else if(baseName == "limit_r") {
           obj = new Limit(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        } else if(baseName == "navigline_l") {
           obj = new NavigLine(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        }else if(baseName == "transmline_l") {
           obj = new TransmissionLine(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        } else if(baseName == "bridge_l") {
           obj = new Bridge(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(baseName), baseName);
        } else if(baseName == "depthcont_l") {
           obj = new DepthContour(layersPixelGeometry.value(layerName), layersCoordinateGeometry.value(layerName),layers.value(layerName), baseName);
        } else {
           qDebug() << Q_FUNC_INFO << "object in unknown layer: " << baseName;
        }

        //this will go boom if the layer name is unknown,
        //we really want this for testing
        qDebug() << Q_FUNC_INFO << "object in layer: " << baseName;
        chartObjects.append(obj);
    }

    foreach (const QString &layerName, chartPointLayers.keys()) {
        const QString &baseName = cleanupLayerName(layerName);

        ChartObjectInterface *chartObj = NULL;
        if (layerName.contains("rock")) {
            chartObj = new Rock(chartPointLayers.value(layerName), layers.value(layerName), baseName);
        } else if (layerName.contains("navaid")) {
           chartObj = new NavaId(chartPointLayers.value(layerName), layers.value(layerName), baseName);
        } else if (layerName.contains("wreck")) {
            chartObj = new Wreck(chartPointLayers.value(layerName),layers.value(layerName), baseName);
        } else if (layerName.contains("signsound")) {
            chartObj = new SignSound(chartPointLayers.value(layerName), layers.value(layerName), baseName);
        }

        if (chartObj) {
            chartObjects.append(chartObj);
        }
    }

    emit modelChanged();
}

void PostgreChartProvider::setZoomLevel(int level){
    zoomLevel = level;
    //request
}

QVector<ChartObjectInterface*> PostgreChartProvider::getChartObjects()
{
    return chartObjects;
}

QRectF PostgreChartProvider::getChartBoundaries()
{
    return chartBoundaries;
}


void PostgreChartProvider::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0)
        layout->addWidget(settingsUI);
}

void PostgreChartProvider::initializeSettings(){

    QFileInfo info(this->getName() + ".xml");

    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct the default settings
        settings->setSetting("User", "postgres");
        settings->setSetting("Password", "upwind");
        settings->setSetting("Port", "5432");
        settings->setSetting("Host", "localhost");
        settings->setSetting("DBName", "chart57");
        settings->setSetting("DetailLevel", "0");
    }
}

QString PostgreChartProvider::getName(){
    return QString("ChartProvider");
}

Settings * PostgreChartProvider::getSettings(){
    return settings;
}

QList<ChartObjectInterface::ChartElement> PostgreChartProvider::getPoints(OGRLayer *layer){
    OGRGeometry *geometry;
    OGRFeature *feature;
    QList<ChartObjectInterface::ChartElement> pointChartElements;

    OGRFeatureDefn *featureDefn = layer->GetLayerDefn();
    QString layerName = featureDefn->GetName();
    int fieldCount = featureDefn->GetFieldCount();

    layer->ResetReading();

    while((feature = layer->GetNextFeature()) != NULL){
        geometry = feature->GetGeometryRef();

        if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbPoint){
            OGRPoint *ogrPoint = (OGRPoint *) geometry;
            QHash<QString, QVariant> attributes;

            for (int i = 0; i < fieldCount; i++) {
                OGRFieldDefn *fieldDefn = featureDefn->GetFieldDefn(i);
                QString fieldName = fieldDefn->GetNameRef();

                if (fieldDefn->GetType() == OFTInteger) {
                    attributes.insert(fieldName, feature->GetFieldAsInteger(i));
                } else if(fieldDefn->GetType() == OFTReal) {
                    attributes.insert(fieldName, feature->GetFieldAsDouble(i));
                } else if (fieldDefn->GetType() == OFTString) {
                    attributes.insert(fieldName, feature->GetFieldAsString(i));
                }
            }

            QPointF point(ogrPoint->getX(), ogrPoint->getY());

            ChartObjectInterface::ChartElement element;
            element.name = layerName;
            element.attributes = attributes;
            element.geoPoint = point;
            geopointToPixel(&point);
            element.pixelPoint = point;

            pointChartElements.append(element);
        }

        /*
         * Case of wkbMultiPoint should be handled somewhere else?
         * Current chart data doesn't seem to return any wkbMultiPoint objects.
         */
        //else if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbMultiPoint){
        //    multiPoint = (OGRMultiPoint *) geometry;
        //    pointCount = multiPoint->getNumGeometries();
        //    for(int i = 0; i < pointCount; ++i){
        //        point = (OGRPoint *) multiPoint->getGeometryRef(i);
        //        QPointF p(point->getX(), point->getY());
        //        geoPoints.append(p);
        //        geopointToPixel(&p);
        //        points.append(p);
        //    }
        //}


        OGRFeature::DestroyFeature(feature);
    }

    return pointChartElements;
}

void PostgreChartProvider::getLines(OGRLayer *layer){
    QVector<QPolygonF> lines;
    QVector<QPolygonF> geoLines;
    QPolygonF tmpLine;
    QPolygonF tmpGeoLine;
    OGRGeometry *geometry;
    OGRFeature *feature;
    OGRMultiLineString *multiLine;
    OGRLineString *line;
    OGRPoint *point;
    int lineCount, pointCount;

    layer->ResetReading();

    while((feature = layer->GetNextFeature()) != NULL){
        geometry = feature->GetGeometryRef();

        if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbLineString){
            line = (OGRLineString *) geometry;
            point = new OGRPoint;

            pointCount = line->getNumPoints();

            for(int i = 0; i < pointCount; ++i){
                line->getPoint(i, point);
                QPointF p(point->getX(), point->getY());
                tmpGeoLine << p;
                geopointToPixel(&p);
                tmpLine << p;
            }
            geoLines.append(tmpGeoLine);
            tmpGeoLine.clear();
            lines.append(tmpLine);
            tmpLine.clear();

            delete(point);
        }
        else if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbMultiLineString){
            multiLine = (OGRMultiLineString *) geometry;
            point = new OGRPoint;

            lineCount = multiLine->getNumGeometries();

            for(int i = 0; i < lineCount; ++i){
                line = (OGRLineString *) multiLine->getGeometryRef(i);

                pointCount = line->getNumPoints();

                for(int j = 0; j < pointCount; ++j){
                    line->getPoint(j, point);
                    QPointF p(point->getX(), point->getY());
                    tmpGeoLine << p;
                    geopointToPixel(&p);
                    tmpLine << p;
                }
                geoLines.append(tmpGeoLine);
                tmpGeoLine.clear();
                lines.append(tmpLine);
                tmpLine.clear();
            }
            delete(point);
        }
        else
            qDebug() << Q_FUNC_INFO << ": No line geometry";
        OGRFeature::DestroyFeature(feature);
    }
    layersPixelGeometry.insert(layer->GetLayerDefn()->GetName(), lines);
    layersCoordinateGeometry.insert(layer->GetLayerDefn()->GetName(), geoLines);
}

void PostgreChartProvider::getPolygons(OGRLayer *layer){
    qDebug() << Q_FUNC_INFO;
    qDebug() << Q_FUNC_INFO << layer->GetLayerDefn()->GetName();

    QVector<QPolygonF> polygons;
    QVector<QPolygonF> geoPolygons;
    QPolygonF tmpPolygon;
    QPolygonF tmpGeoPolygon;
    OGRGeometry *geometry;
    OGRFeature *feature;
    OGRMultiPolygon *multiPoly;
    OGRPolygon *poly;
    OGRLinearRing *insideRing, *outerRing;
    OGRPoint *point;
    int polyCount, ringCount, pointCount;

    layer->ResetReading();

    while((feature = layer->GetNextFeature()) != NULL){
        geometry = feature->GetGeometryRef();

        if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbPolygon){
            poly = (OGRPolygon *) geometry;
            point = new OGRPoint;

            ringCount = poly->getNumInteriorRings();

            for(int i = 0; i < ringCount; ++i){
                insideRing = poly->getInteriorRing(i);
                pointCount = insideRing->getNumPoints();

                for(int j = pointCount - 1; j >= 0; --j){
                    insideRing->getPoint(j, point);
                    QPointF p(point->getX(), point->getY());
                    tmpGeoPolygon << p;

                    geopointToPixel(&p);

                    tmpPolygon << p;
                }
                geoPolygons.append(tmpGeoPolygon);
                tmpGeoPolygon.clear();
                polygons.append(tmpPolygon);
                tmpPolygon.clear();
            }

            outerRing = poly->getExteriorRing();
            pointCount = outerRing->getNumPoints();

            for(int j = pointCount - 1; j >= 0; --j){
                outerRing->getPoint(j, point);
                QPointF p(point->getX(), point->getY());
                tmpGeoPolygon << p;
                geopointToPixel(&p);

                tmpPolygon << p;
            }
            geoPolygons.append(tmpGeoPolygon);
            tmpGeoPolygon.clear();
            polygons.append(tmpPolygon);
            tmpPolygon.clear();

            delete(point);
        }
        else if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon){
            multiPoly = (OGRMultiPolygon *) geometry;
            point = new OGRPoint;

            polyCount = multiPoly->getNumGeometries();

            for(int i = 0; i < polyCount; ++i){
                poly = (OGRPolygon *) multiPoly->getGeometryRef(i);

                ringCount = poly->getNumInteriorRings();

                for(int j = 0; j < ringCount; ++j){
                    insideRing = poly->getInteriorRing(j);
                    pointCount = insideRing->getNumPoints();

                    for(int k = pointCount - 1; k >= 0; --k){
                        insideRing->getPoint(k, point);
                        QPointF p(point->getX(), point->getY());
                        tmpGeoPolygon << p;
                        geopointToPixel(&p);
                        tmpPolygon << p;
                    }
                    geoPolygons.append(tmpGeoPolygon);
                    tmpGeoPolygon.clear();
                    polygons.append(tmpPolygon);
                    tmpPolygon.clear();
                }

                outerRing = poly->getExteriorRing();
                pointCount = outerRing->getNumPoints();

                for(int j = pointCount - 1; j >= 0; --j){
                    outerRing->getPoint(j, point);
                    QPointF p(point->getX(), point->getY());
                    tmpGeoPolygon << p;
                    geopointToPixel(&p);
                    tmpPolygon << p;
                }
                geoPolygons.append(tmpGeoPolygon);
                tmpGeoPolygon.clear();
                polygons.append(tmpPolygon);
                tmpPolygon.clear();
            }
            delete(point);
        }
        else
            qDebug() << Q_FUNC_INFO << ": No polygons geometry";
        OGRFeature::DestroyFeature(feature);
    }

    layersPixelGeometry.insert(layer->GetLayerDefn()->GetName(), polygons);
    layersCoordinateGeometry.insert(layer->GetLayerDefn()->GetName(), geoPolygons);
}

void PostgreChartProvider::getDepths(OGRLayer *layer){

    OGRGeometry *geometry;
    OGRFeature *feature;
    OGRMultiPolygon *multiPoly;
    OGRPolygon *poly;

    int polyCount, ringCount;

    layer->ResetReading();

    while((feature = layer->GetNextFeature()) != NULL){
        geometry = feature->GetGeometryRef();

        if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbPolygon){
            poly = (OGRPolygon *) geometry;
            OGRFeatureDefn *poFDefn = layer->GetLayerDefn();

            for(int j = 0; j < poFDefn->GetFieldCount(); j++ ){
                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( j );
                 QString str = poFieldDefn->GetNameRef() ;
                 if (str.contains("mindepth") == true) {
                    ringCount = poly->getNumInteriorRings();

                    for(int i = 0; i < ringCount; ++i){
                        depths.append(feature->GetFieldAsDouble(j));
                    }
                 }
            }

            for(int i = 0; i < poFDefn->GetFieldCount(); i++ ){
                OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( i );
                 QString str = poFieldDefn->GetNameRef() ;
                 if (str.contains("mindepth") == true) {
                     depths.append(feature->GetFieldAsDouble(i));
                 }
            }
        }
        else if(geometry != NULL && wkbFlatten(geometry->getGeometryType()) == wkbMultiPolygon){
            multiPoly = (OGRMultiPolygon *) geometry;
            polyCount = multiPoly->getNumGeometries();
            for(int i = 0; i < polyCount; ++i){
                poly = (OGRPolygon *) multiPoly->getGeometryRef(i);
                ringCount = poly->getNumInteriorRings();

                OGRFeatureDefn *poFDefn = layer->GetLayerDefn();

                for(int j = 0; j < ringCount; ++j){

                    for(int j = 0; j < poFDefn->GetFieldCount(); j++ ){
                        OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( j );
                         QString str = poFieldDefn->GetNameRef() ;
                         if (str.contains("mindepth") == true) {
                                depths.append(feature->GetFieldAsDouble(j));
                         }
                    }

                }

                for(int i = 0; i < poFDefn->GetFieldCount(); i++ ){
                    OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( i );
                     QString str = poFieldDefn->GetNameRef() ;
                     if (str.contains("mindepth") == true) {
                         depths.append(feature->GetFieldAsDouble(i));
                     }
                }
            }

        }
        else
            OGRFeature::DestroyFeature(feature);
    }

}

QString PostgreChartProvider::getConName(){
    return dbName;
}

QString PostgreChartProvider::getConUser(){
    return dbUser;
}

QString PostgreChartProvider::getConPass(){
    return dbPass;
}

QString PostgreChartProvider::getConPort(){
    return QString::number(dbPort);
}

QString PostgreChartProvider::getConHost(){
    return dbHost;
}

void PostgreChartProvider::setConName(QString name){
    dbName = name;
}

void PostgreChartProvider::setConUser(QString user){
    dbUser = user;
}

void PostgreChartProvider::setConPass(QString pass){
    dbPass = pass;
}

void PostgreChartProvider::setConPort(uint port){
    dbPort = port;
}

void PostgreChartProvider::setConHost(QString host){
    dbHost = host;
}

void PostgreChartProvider::setDetailLevel(int level) {
    detailLevel = level;
}

void PostgreChartProvider::setChartWidgetSize(QSize size){
       chartSize = size;
}

inline void PostgreChartProvider::geopointToPixel(QPointF *geopoint){
    UwMath::toConformalInverted(geopoint);

    geopoint->setX((geopoint->x() - chartBoundaries.left()) * (chartSize.width() / chartBoundaries.width()));
    geopoint->setY((geopoint->y() - chartBoundaries.top()) * (chartSize.height() / chartBoundaries.height()));
}

inline QString PostgreChartProvider::cleanupLayerName(QString layerName) {
    QString baseName;
    int endOfNamePosition;

    if(layerName.count("_") > 1) {
        endOfNamePosition = layerName.lastIndexOf("_");
        baseName = layerName.leftRef(endOfNamePosition).toString();
    }
    else
        baseName = layerName;

    return baseName;
}


Q_EXPORT_PLUGIN2(postgrechartprovider, PostgreChartProvider)
