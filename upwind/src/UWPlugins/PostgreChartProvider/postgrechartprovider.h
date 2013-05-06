#ifndef POSTGRECHARTPROVIDER_H
#define POSTGRECHARTPROVIDER_H

#include "../ChartProviderInterface/corechartprovider.h"
#include "../ChartProviderInterface/chartobjectinterface.h"
#include "settingsui.h"

#include <QPolygon>
#include <QVector>
#include <QObject>
#include <QSize>

class OGRDataSource;
class OGRLayer;
class SettingsUI;
class Settings;

class PostgreChartProvider : public CoreChartProvider
{
    Q_OBJECT

public:
    PostgreChartProvider();

    virtual ~PostgreChartProvider();

    void initialize(QString host,
                    uint port,
                    QString dbname,
                    QString username,
                    QString password,
                    int detailLevel);

    virtual void addPluginSettingsToLayout(QLayout *layout);

    virtual QString getName();

    void initializeSettings();

    virtual Settings * getSettings();

    QVector<ChartObjectInterface*> getChartObjects();
    QRectF getChartBoundaries();

    void setAreaFilter(QRectF area);

    void setZoomLevel(int level);

    void setConName(QString name);
    void setConUser(QString user);
    void setConPass(QString pass);
    void setConPort(uint port);
    void setConHost(QString host);
    void setDetailLevel(int level);

    void setChartWidgetSize(QSize size);

    QString getConName();
    QString getConUser();
    QString getConPass();
    QString getConPort();
    QString getConHost();
    QSet<QString> getSelectedLayers();

    /**
      * This method gets only one layer from database. The wanted layer is specified
      * by the calling method. This makes fetching the map faster since unused layers are not fetched.
      */
    OGRLayer* getLayerLevel(QString layerNameString, int layerNumber);

private:
    virtual void initConnection();

    /** This method crates a structures that maps the database
      * layers to their names. This should be done just after connecting
      * and can be used also to filter the layers so the mapping
      * contains only the layers we're interested in using (for planning
      * a route or displaying the chart).
      */
    void mapLayers();

    /** This method creates a list of points stored as a qpolygon,
      * contained in an ogr layer. the layer can contain a spacial filter,
      * so there's no need for setting it up in the function. The points returned
      * are in the mercator projection of the earth in radians,
      * the latitudes have been inverted, so the north
      * pole has the lowest value of all for easier rendering.
      */
    QList<ChartObjectInterface::ChartElement> getPoints(OGRLayer *layer);

    /** This method creates a list of lines contained in an ogr layer
      * the layer can contain a spacial filter, so there's no need for setting
      * it up in the function. The points returned are in the mercator projection
      * of the earth in radians, the latitudes have been inverted, so the north
      * pole has the lowest value of all for easier rendering.
      */
    void getLines(OGRLayer *layer);

    /** This method creates a list of poligons contained in an ogr layer
      * the layer can contain a spacial filter, so there's no need for setting
      * it up in the function. The points returned are in the mercator projection
      * of the earth in radians, the latitudes have been inverted, so the north
      * pole has the lowest value of all for easier rendering.
      * TODO consider inner rings to be separate of the outer rings so they can be
      * processed/rendered at different times or in different ways. (to display
      * different colours, for example)
      */
    void getPolygons(OGRLayer *layer);

    /** This method sets the square in the mercator projection
      * where the chart is placed this class is connected is placed.
      * It is used to determine the offsets used to transform form a point
      * in the chart to a pixel in the chart widget.
      */
    void setBoundingBox(OGRLayer *layer);

    /** This method transforms a geographical point to a pixel
      * in the chart widget. Right now it stretches the map, it
      * doesn't maintain aspect ratio of the map projection.
      */
    void geopointToPixel(QPointF *geopoint);

    /** This function returns the name of the OGRLayer without the
      * trailing "_levelx".
      */
    QString cleanupLayerName(QString LayerName);

    void getDepths(OGRLayer *layer);


private:
    SettingsUI *settingsUI;
    OGRDataSource *dataSource;
    QMap<QString, OGRLayer *> layers;
    QMap<QString, QVector<QPolygonF> > layersCoordinateGeometry;
    QMap<QString, QVector<QPolygonF> > layersPixelGeometry;
    QSet<QString> selectedLayers;
    QRectF filter;
    QRectF chartBoundaries;
    int zoomLevel;

    /** Dictates amount of details we want to fetch
      * from the database. It is used to select the layers
      * that contain a certain amount of detail.
      */
    int detailLevel;
    QVector<ChartObjectInterface *> chartObjects;
    QString dbName;
    QString dbUser;
    QString dbPass;
    uint dbPort;
    QString dbHost;
    QSize chartSize;

    QVector<double> depths;

protected:
    Settings *settings;
};

#endif // POSTGRECHARTPROVIDER_H
