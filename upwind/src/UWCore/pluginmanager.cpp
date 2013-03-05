#include "pluginmanager.h"
#include "uwcore.h"
#include "../UWPlugins/ChartProviderInterface/corechartprovider.h"
#include <QtGui>
#include <typeinfo>


PluginManager::PluginManager(){
    viewRenderer = 0;
    nmeaReader = 0;
    chartProvider = 0;
    upWindScene = 0;
    routeManager = 0;
    logger = 0;
}

PluginManager::~PluginManager(){}

QString PluginManager::getPluginInfoString(){
    QString pluginInfoString = "";
    QString routeManagers = "<Route Managers>\n";
    QString nmeaReaders = "<NMEA Readers>\n";
    QString loggers = "<Loggers>\n";
    QString chartProviders = "<Chart Providers>\n";
    QString viewRenderers = "<View Renderers>\n";
    QString upwindScenes = "<UpWind Scenes>\n";
    QString instrumentStrings = "<Instruments>\n";

    foreach(UWPluginInterface *uwPlugin, this->loadedPlugins){
        NMEAReaderInterface * nmeaR = dynamic_cast<NMEAReaderInterface *>(uwPlugin);
        RouteManagerInterface * routeM = dynamic_cast<RouteManagerInterface *>(uwPlugin);
        LoggerInterface * loggr = dynamic_cast<LoggerInterface *>(uwPlugin);
        ChartProviderInterface * chartR = dynamic_cast<ChartProviderInterface *>(uwPlugin);
        ViewRendererInterface * viewR = dynamic_cast<ViewRendererInterface *>(uwPlugin);
        UpWindSceneInterface * uScene = dynamic_cast<UpWindSceneInterface *>(uwPlugin);
        NMEAInstrumentInterface * instru = dynamic_cast<NMEAInstrumentInterface *>(uwPlugin);

        if(instru){
            instrumentStrings += "\t" + instru->getName();
            instrumentStrings += "\n";
        }
        else if(routeM){
            if(routeM == routeManager)
                routeManagers += "\t[" + routeM->getName() + "]\n";
            else
                routeManagers += "\t" + routeM->getName() + "\n";
        }
        else if(nmeaR){
            if(nmeaR == nmeaReader)
                nmeaReaders += "\t[" + nmeaR->getName() + "]\n";
            else
                nmeaReaders += "\t" + nmeaR->getName() + "\n";
        }
        else if(loggr){
            if(loggr == logger)
                loggers += "\t[" + loggr->getName() + "]\n";
            else
                loggers += "\t" + loggr->getName() + "\n";
        }
        else if(chartR){
            if(chartR == chartProvider)
                chartProviders += "\t[" + chartR->getName() + "]\n";
            else
                chartProviders += "\t" + chartR->getName() + "\n";
        }
        else if(viewR){
            if(viewR == viewRenderer)
                viewRenderers += "\t[" + viewR->getName() + "]\n";
            else viewRenderers += "\t" + viewR->getName() + "\n";
        }
        else if(uScene){
            if(uScene == upWindScene)
                upwindScenes += "\t[" + uScene->getName() + "]\n";
            else
                upwindScenes += "\t" + uScene->getName() + "\n";
        }
    }

    pluginInfoString += "\n" + viewRenderers + "\n" + nmeaReaders + "\n" + chartProviders + "\n" + upwindScenes + "\n" + loggers + "\n" + routeManagers + "\n" + instrumentStrings;
    qDebug()<<Q_FUNC_INFO << "pluginInfo:" << pluginInfoString;
    return pluginInfoString;
}

void PluginManager::appClosing(){
    if(this->logger)
        logger->saveBeforeQuit();
}

void PluginManager::makeConnections(){
    //The QObject::connects - this whole plugin connect/disconnect thing could use redesigning.
    if (nmeaReader)
        connectNMEAReader(nmeaReader);

    if(viewRenderer)
        connectViewRenderer(viewRenderer);

    if(upWindScene)
        connectUpWindScene(upWindScene);

    if(routeManager)
        routeManager->connectRouteManager(upWindScene, getInstruments());
}

void PluginManager::connectUpWindScene(UpWindSceneInterface* uwscene){
    CoreUpWindScene *uwScene = static_cast<CoreUpWindScene*>(uwscene);
    CoreChartProvider *chartObjects = static_cast<CoreChartProvider *>(chartProvider);
    uwScene->setChartObjects(chartObjects);
}

void PluginManager::connectNMEAReader(NMEAReaderInterface* nmeaR){
    CoreNMEAReader *reader = static_cast<CoreNMEAReader *>(nmeaR);
    connect(reader, SIGNAL(newNMEAString(QString)), this, SLOT(manageNMEAString(QString)), Qt::QueuedConnection);
}

void PluginManager::disconnectNMEAReader(NMEAReaderInterface *nmeaR){
    CoreNMEAReader *reader = static_cast<CoreNMEAReader *>(nmeaR);
    disconnect(reader, SIGNAL(newNMEAString(QString)), this, SLOT(manageNMEAString(QString)));
}

void PluginManager::connectViewRenderer(ViewRendererInterface *viewR){
    CoreViewRenderer *render = static_cast<CoreViewRenderer *>(viewR); //TODO dynamic_cast fails in OSX.
    CoreChartProvider *provider = static_cast<CoreChartProvider *>(chartProvider);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomInS()), render, SLOT(zoomIn()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomOutS()), render, SLOT(zoomOut()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomBoat()), render, SLOT(zoomBoat()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomChart()), render, SLOT(expand()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(rotateLeftS()), render, SLOT(rotateLeft()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(rotateRightS()), render, SLOT(rotateRight()), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomToolActivated(bool)), render, SLOT(zoomToolActivated(bool)), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(simModeChanged(bool)), render, SLOT(simModeChanged(bool)), Qt::QueuedConnection);
    connect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(drawRoute(bool)), render, SLOT(drawRoute(bool)), Qt::QueuedConnection);


    if(upWindScene && provider){
        chartProvider->setChartWidgetSize(UWCore::getInstance()->getUIManager()->getMap()->size());
        viewR->ConnectPlugin(upWindScene, UWCore::getInstance()->getUIManager()->getMap(), provider);
        UWCore::getInstance()->getUIManager()->getMap()->lower();


    }
}

void PluginManager::disconnectViewRenderer(ViewRendererInterface *viewR){
    CoreViewRenderer *render = dynamic_cast<CoreViewRenderer *>(viewR);
    disconnect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomInS()), render, SLOT(zoomIn()));
    disconnect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomOutS()), render, SLOT(zoomOut()));
    disconnect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomBoat()), render, SLOT(zoomBoat()));
    disconnect(UWCore::getInstance()->getUIManager()->getToolBox(), SIGNAL(zoomChart()), render, SLOT(expand()));

    //the plugin's chart view should be removed here..
}

void PluginManager::savePluginSettings(){
    foreach(UWPluginInterface* plugin, loadedPlugins)
        if(!dynamic_cast<NMEAInstrumentInterface *>(plugin))
            if(plugin->getSettings())
                plugin->getSettings()->saveSettings();
}

void PluginManager::saveInstrumentPositions(){
    foreach(NMEAInstrumentInterface * instrument, getInstruments())
        instrument->savePosition();
}

void PluginManager::loadPlugins(){
    QDir pluginsDir(qApp->applicationDirPath());
    qDebug() << Q_FUNC_INFO << qApp->applicationDirPath();

#if defined(Q_OS_WIN)
    qDebug()<<Q_FUNC_INFO<<"Q_OS_WIN";
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
    {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#elif defined(Q_OS_MAC)
    qDebug() << Q_FUNC_INFO<<"Q_OS_MAC";
    if (pluginsDir.dirName() == "MacOS"){
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#elif defined(Q_OS_LINUX)
    qDebug() << Q_FUNC_INFO << "Q_OS_LINUX";
    pluginsDir.cdUp();
#endif

    qDebug() << Q_FUNC_INFO << "Dir: " << pluginsDir.dirName();
    pluginsDir.cd("UWPlugins");
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        // THIS HAS NO SECURITY WATSOEVER AND CAN BE EXPLOITED BADLY
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        qDebug() << Q_FUNC_INFO << fileName << " " << plugin << (plugin == 0 ? pluginLoader.errorString() : "Success");

        if(plugin) {
            if(qobject_cast<UWPluginInterface *>(plugin))
                loadedPlugins.insert(qobject_cast<UWPluginInterface *>(plugin));

            if(qobject_cast<ViewRendererInterface *>(plugin))
                loadedRenderers.insert(qobject_cast<ViewRendererInterface *>(plugin));

            else if(qobject_cast<ChartProviderInterface *>(plugin))
                loadedChartProviders.insert(qobject_cast<ChartProviderInterface *>(plugin));

            else if(qobject_cast<NMEAReaderInterface *>(plugin))
                loadedNmeaProviders.insert(qobject_cast<NMEAReaderInterface *>(plugin));

            else if(qobject_cast<UpWindSceneInterface *>(plugin))
                loadedScenes.insert(qobject_cast<UpWindSceneInterface *>(plugin));

            else if(qobject_cast<RouteManagerInterface *>(plugin))
                loadedRouteManagers.insert(qobject_cast<RouteManagerInterface *>(plugin));

            else if(qobject_cast<NMEAInstrumentInterface *>(plugin)){
                NMEAInstrumentInterface *instrument = qobject_cast<NMEAInstrumentInterface *>(plugin);
                instruments.insert(instrument->getName(), instrument);
            }
        }
    }

    //Plugins are now loaded. Now let's select the ones to use.
    //if the settings file is not up to date or doesn't exist - choose the first plugin that kind
    //notice that connection order of the plugins is important
    Settings * selectedPlugins = UWCore::getInstance()->getSettingsManager()->getSelectedPlugins();

    if(getPlugin(selectedPlugins->getSetting("NMEAReader")))
        setNMEAReader(getPlugin(selectedPlugins->getSetting("NMEAReader")));
    if(nmeaReader == 0 && !loadedNmeaProviders.isEmpty())
        setNMEAReader(*(loadedNmeaProviders.begin()));

    if(getPlugin(selectedPlugins->getSetting("ChartProvider")))
        setChartProvider(getPlugin(selectedPlugins->getSetting("ChartProvider")));
    if(chartProvider == 0 && !loadedChartProviders.isEmpty())
        setChartProvider(*(loadedChartProviders.begin()));

    if(getPlugin(selectedPlugins->getSetting("Logger")))
        setLogger(getPlugin(selectedPlugins->getSetting("Logger")));
    if(logger == 0 && !loadedLoggers.isEmpty())
        setLogger(*(loadedLoggers.begin()));

    if(getPlugin(selectedPlugins->getSetting("RouteManager")))
        setRouteManager(getPlugin(selectedPlugins->getSetting("RouteManager")));
    if(routeManager == 0 && !loadedRouteManagers.isEmpty())
        setRouteManager(*(loadedRouteManagers.begin()));

    if(getPlugin(selectedPlugins->getSetting("UpWindScene")))
        setUpWindScene(getPlugin(selectedPlugins->getSetting("UpWindScene")));
    if(upWindScene == 0 && !loadedScenes.isEmpty())
        setUpWindScene(*(loadedScenes.begin()));

    if(getPlugin(selectedPlugins->getSetting("Renderer")))
        setViewRenderer(getPlugin(selectedPlugins->getSetting("Renderer")));
    if(viewRenderer == 0 && !loadedRenderers.isEmpty())
        setViewRenderer(*(loadedRenderers.begin()));
}

void PluginManager::setChartProvider(UWPluginInterface * plugin){
    ChartProviderInterface *newChartProvider = dynamic_cast<ChartProviderInterface *>(plugin);
    if(chartProvider != newChartProvider)
        chartProvider = newChartProvider;
}

void PluginManager::setLogger(UWPluginInterface * plugin){
    LoggerInterface *newLogger = dynamic_cast<LoggerInterface *>(plugin);
    if(logger != newLogger)
        logger = newLogger;
}

void PluginManager::setNMEAReader(UWPluginInterface * plugin){
    NMEAReaderInterface *newNMEAReader = dynamic_cast<NMEAReaderInterface *>(plugin);

    if(nmeaReader != newNMEAReader)
        if(nmeaReader){
            //if the old reader is not null, disconnect it and connect the new one
            connectNMEAReader(newNMEAReader);
            disconnectNMEAReader(nmeaReader);
        }

    nmeaReader = newNMEAReader;
}

void PluginManager::setRouteManager(UWPluginInterface * plugin){
    RouteManagerInterface *newRouteManager = dynamic_cast<RouteManagerInterface *>(plugin);
    if(routeManager != newRouteManager)
        routeManager = newRouteManager;

}

void PluginManager::setUpWindScene(UWPluginInterface * plugin){
    UpWindSceneInterface *newUpwindScene = dynamic_cast<UpWindSceneInterface *>(plugin);
    if(upWindScene != newUpwindScene)
        upWindScene = newUpwindScene;
}

void PluginManager::setViewRenderer(UWPluginInterface * plugin){
    ViewRendererInterface *newViewRenderer = dynamic_cast<ViewRendererInterface *>(plugin);
    if(viewRenderer != newViewRenderer){
        if(viewRenderer){
            //if the old renderer is not null, disconnect it and connect the new one
            QMessageBox::about(0, "Connecting", newViewRenderer->getName());
            connectViewRenderer(newViewRenderer);
            disconnectViewRenderer(viewRenderer);
        }
        viewRenderer = newViewRenderer;
    }
}

void PluginManager::manageNMEAString(const QString &nmeaString) {
    //send to the XML plugin
    //send to the Instruments
    //send to the Scene
    //emit paintInstrumentChanged(nmeaString);
    QList<NMEAInstrumentInterface *> instruments = getInstruments();
    foreach(NMEAInstrumentInterface *ins, instruments)
        ins->parseNMEAString(nmeaString);
    if(logger)
        logger->appendNMEAString(nmeaString);

    upWindScene->parseNMEAString(nmeaString);
}

RouteManagerInterface * PluginManager::getRouteManager(){
    return routeManager;
}

LoggerInterface * PluginManager::getLogger(){
    return logger;
}

UWPluginInterface * PluginManager::getPlugin(const QString &pluginName){
    foreach(UWPluginInterface* plugin, loadedPlugins)
        if(plugin->getName() == pluginName)
            return plugin;

    return 0;
}

NMEAInstrumentInterface * PluginManager::getInstrument(const QString &instrumentName){
    if(instruments.contains(instrumentName))
        return instruments.value(instrumentName);

    return 0;
}

QSet<UWPluginInterface *> PluginManager::getLoadedPlugins(){
    return loadedPlugins;
}

ViewRendererInterface * PluginManager::getViewRenderer(){
    return viewRenderer;
}

ChartProviderInterface * PluginManager::getChartProvider(){
    return chartProvider;
}

NMEAReaderInterface * PluginManager::getNMEAReader(){
    return nmeaReader;
}

UpWindSceneInterface * PluginManager::getUpWindScene(){
    return upWindScene;
}

QList<NMEAInstrumentInterface *> PluginManager::getInstruments(){
    return instruments.values();
}
