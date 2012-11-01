#ifndef COREUPWINDSCENE_H
#define COREUPWINDSCENE_H

#include <QObject>
#include <QLayout>
#include <QtXml/QDomDocument>

#include "UpWindSceneInterface.h"
#include "settingsui.h"
#include "Scene/boat.h"
#include "../ChartProviderInterface/corechartprovider.h"
#include "Scene/route.h"
#include "Scene/shortnavig.h"
#include "../ChartProviderInterface/chartproviderinterface.h"

class Route;

class CoreUpWindScene: public QObject, public UpWindSceneInterface
{
    Q_OBJECT
    Q_INTERFACES(UpWindSceneInterface UWPluginInterface)

public:
    CoreUpWindScene();

    ~CoreUpWindScene();

    /** Adds the plugin to the given layout. The plugin will no longer be on it's own window.
      * @param layout - Pointer to a QLayout to which the plugin widget (UI) will be added
      */
    void addPluginSettingsToLayout(QLayout *layout);
    void setChartObjects(CoreChartProvider *model);


    /** Parse the NMEA string if it is a GPS NMEA string
      * @param text - NMEA string
      */
    void parseNMEAString( const QString & text);



    /** Returns the name of the plugin
      * @return name of the plugin as a QString
      */
    QString getName();
    Settings* getSettings();
    Boat* getBoat();
    Route* getRoute();
    ShortNavigation* getShortNavigation();
    void initializeSettings();
    Route *route;
    ShortNavigation *shortNavigation;
    void setBoat(Boat *boat);

private:

    void fetchChartObjects();

    CoreChartProvider *model;
    QVector<ChartObjectInterface*> chartObjects;
    Boat *boat;

    //ShortNavigation *shortnavig;
    SettingsUI *settingsUI;
    Settings *settings;

    QStringList parsedNMEAValues;
    float latitude;
    float longitude;

protected slots:

signals:
};

#endif // COREUPWINDSCENE_H
