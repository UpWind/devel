#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "../UWPlugins/ChartProviderInterface/chartproviderinterface.h"
#include "../UWPlugins/ViewRenderer/viewrendererinterface.h"
#include "../UWPlugins/NMEAReader/nmeareaderinterface.h"
#include "../UWPlugins/UWPluginInterface/uwplugininterface.h"
#include "../UWPlugins/UpWindScene/UpWindSceneInterface.h"
#include "../UWPlugins/UpWindScene/coreupwindscene.h"
#include "../UWPlugins/RouteManager/RouteManagerInterface.h"
#include "../UWPlugins/Logger/LoggerInterface.h"
#include "../UWPlugins/NMEAInstruments/CoreNMEAInstrument/nmeainstrumentinterface.h"

#include "../UWPlugins/NMEAReader/corenmeareader.h"
#include "../UWPlugins/ViewRenderer/coreviewrenderer.h"
#include "../NMEADataSimulator/datasimulatorcontrolinterface.h"
#include "../NMEAInstruments/SimulatorWheel/simulatorwheelinterface.h"

class PluginManager : public QObject
{
    Q_OBJECT

public:

    /**
     * Constructor of the PluginManager.
     */
    explicit PluginManager();

    /**
     * Destructor of the PluginManager.
     */
    ~PluginManager();

    /**
     * Loads the plugins according to the selected plugins-settings of settings manager.
     */
    void loadPlugins();

    void makeConnections();

    /**
     * Getter function of the viewRenderer attribute.
     * @return pointer to the active view renderer.
     */
    ViewRendererInterface * getViewRenderer();

    /**
     * Getter function of the ChartProvider attribute.
     * @return pointer to the active chart provider.
     */
    ChartProviderInterface * getChartProvider();

    /**
     * Getter function of the nmeaReader attribute.
     * @return pointer to the active NMEA reader.
     */
    NMEAReaderInterface * getNMEAReader();

    /**
     * Getter function of the upWindScene attribute.
     * @return pointer to the active upwind scene.
     */
    UpWindSceneInterface * getUpWindScene();

    /**
     * Getter function of the routeManager attribute.
     * @return pointer to the active route manager.
     */
    RouteManagerInterface * getRouteManager();

    /**
     * Getter function of the logger attribute.
     * @return pointer to the active logger.
     */
    LoggerInterface * getLogger();

    /**
     * Getter function of the instruments.
     * @return QList with pointers to all of the instruments.
     */
    QList<NMEAInstrumentInterface*> getInstruments();

    /**
     * Getter function for a single instrument.
     * @param the name of the required instrument.
     * @return pointer to the instrument or 0 if the instrument is not found.
     */
    NMEAInstrumentInterface * getInstrument( const QString &instrumentName );

    /**
     * Returns a formatted string with information about the loaded
     * plugins and which ones are in use.
     *@return QString containing info about the plugins.
     */
    QString getPluginInfoString();

    /**
     * Setter function of the viewRenderer attribute.
     * @param pointer to the view renderer.
     */
    void setViewRenderer(UWPluginInterface*);

    /**
     * Setter function of the ChartProvider attribute.
     * @param pointer to the chart provider.
     */
    void setChartProvider(UWPluginInterface*);

    /**
     * Setter function of the nmeaReader attribute.
     * @param pointer to the NMEA reader.
     */
    void setNMEAReader(UWPluginInterface*);

    /**
     * Setter function of the upWindScene attribute.
     * @param pointer to the upwind scene.
     */
    void setUpWindScene(UWPluginInterface*);

    /**
     * Setter function of the routeManager attribute.
     * @param pointer to the route manager.
     */
    void setRouteManager(UWPluginInterface*);

    /**
     * Setter function of the logger attribute.
     * @param pointer to the logger.
     */
    void setLogger(UWPluginInterface*);

    /**
     * Cleanup function that's called when the application is closing.
     */
    void appClosing();

    /**
      * Returns a single plugin defined by name.
      * @param the name of the plugin.
      * @return pointer to the plugin.
      */
    UWPluginInterface * getPlugin(const QString &pluginName);

    /**
      * Returns a list containing pointers to all of the loaded plugins.
      *@return QList with pointers to all of the loaded plugins.
      */
    QSet<UWPluginInterface*>  getLoadedPlugins();

    /**
     * Saves the positions of the instruments.
     */
    void saveInstrumentPositions();

    void savePluginSettings();


private:
    void connectUpWindScene(UpWindSceneInterface* uwscene);
    void connectNMEAReader(NMEAReaderInterface* nmeaR);
    void connectViewRenderer(ViewRendererInterface *viewR);
    void disconnectNMEAReader(NMEAReaderInterface* nmeaR);
    void disconnectViewRenderer(ViewRendererInterface *viewR);

    ViewRendererInterface *viewRenderer;
    QSet<ViewRendererInterface *> loadedRenderers;

    ChartProviderInterface *chartProvider;
    QSet<ChartProviderInterface *> loadedChartProviders;

    DataSimulatorControlInterface *dataSimulatorControlInterface;
    SimulatorWheelInterface *simulatorWheel;

    NMEAReaderInterface *nmeaReader;
    QSet<NMEAReaderInterface *> loadedNmeaProviders;

    UpWindSceneInterface *upWindScene;
    QSet<UpWindSceneInterface *> loadedScenes;

    RouteManagerInterface *routeManager;
    QSet<RouteManagerInterface *> loadedRouteManagers;

    LoggerInterface *logger;
    QSet<LoggerInterface *> loadedLoggers;

    //TODO: deprecate loadedPlugins, it's redundant data
    QSet<UWPluginInterface *> loadedPlugins;

    QMap<QString, NMEAInstrumentInterface *> instruments;

    void connectWheelAndSimulator();

signals:
    void paintInstrumentChanged(const QString &nmeaString);

public slots:

    /**
     * Handles the nmea strings coming from the NMEA reader.
     */
    void manageNMEAString(const QString &nmeaString);
};

#endif // PLUGINMANAGER_H
