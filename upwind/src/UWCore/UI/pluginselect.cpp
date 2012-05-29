#include "pluginselect.h"
#include "ui_pluginselect.h"
#include "uwcore.h"
#include <QList>
#include <QTextEdit>

#include "ChartProviderInterface/chartproviderinterface.h"
#include "ViewRenderer/viewrendererinterface.h"
#include "NMEAReader/nmeareaderinterface.h"
#include "UWPluginInterface/uwplugininterface.h"
#include "UpWindScene/UpWindSceneInterface.h"
#include "RouteManager/RouteManagerInterface.h"
#include "Logger/LoggerInterface.h"
#include "NMEAInstruments/CoreNMEAInstrument/nmeainstrumentinterface.h"

const QString NMEA_READERS_STRING = "NMEA Readers";
const QString VIEW_RENDERERS_STRING = "View Renderers";
const QString CHART_PROVIDERS_STRING = "Chart Providers";
const QString UPWIND_SCENES_STRING = "UpWind Scenes";
const QString LOGGERS_STRING = "Loggers";
const QString INSTRUMENTS_STRING = "Instruments";
const QString ROUTE_MANAGERS_STRING = "Route Managers";

PluginSelect::PluginSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginSelect)
{
    ui->setupUi(this);

    this->selectedIcon= new QIcon(":/button_ok.png");
    this->notSelectedIcon = new QIcon(":/button_cancel.png");

    rendererBox = ui->rendererBox;
    nmeaReaderBox = ui->nmeaReaderBox;
    chartProviderBox = ui->chartProviderBox;
    routeManagerBox = ui->routeManagerBox;
    loggerBox = ui->loggerBox;
    upwindSceneBox = ui->upwindSceneBox;

    updateTextBox();
    setupUI();
}

PluginSelect::~PluginSelect(){
    delete ui;
}

void PluginSelect::updateTextBox(){
    ui->textEdit->clear();
    ui->textEdit->append(UWCore::getInstance()->getPluginManager()->getPluginInfoString());

}

void PluginSelect::updateTreeView(const QString &pluginName){
    UWPluginInterface* plugin = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    ChartProviderInterface* chartProvider = dynamic_cast<ChartProviderInterface *>(plugin);
    ViewRendererInterface* viewRenderer = dynamic_cast<ViewRendererInterface *>(plugin);
    NMEAReaderInterface* nmeaReader = dynamic_cast<NMEAReaderInterface *>(plugin);
    RouteManagerInterface* routeManager = dynamic_cast<RouteManagerInterface *>(plugin);
    LoggerInterface* logger = dynamic_cast<LoggerInterface *>(plugin);
    UpWindSceneInterface* upwindScene = dynamic_cast<UpWindSceneInterface *>(plugin);

    if(chartProvider){
        for(int i = 0; i < this->chartProviderItem->childCount(); i++){
            QTreeWidgetItem * item = this->chartProviderItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else
                item->setIcon(0, *notSelectedIcon);
        }
    } else if(viewRenderer){
        for(int i = 0; i < this->rendererItem->childCount(); i++){
            QTreeWidgetItem * item = this->rendererItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else
                item->setIcon(0, *notSelectedIcon);
        }
    } else if(nmeaReader){
        for(int i = 0; i < this->nmeaReaderItem->childCount(); i++){
            QTreeWidgetItem * item = this->nmeaReaderItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else
                item->setIcon(0, *notSelectedIcon);
        }
    } else if(routeManager){
        for(int i = 0; i < this->routeManagerItem->childCount(); i++){
            QTreeWidgetItem * item = this->routeManagerItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else
                item->setIcon(0, *notSelectedIcon);
        }
    } else if(logger){
        for(int i = 0; i < this->loggerItem->childCount(); i++){
            QTreeWidgetItem * item = this->loggerItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else
                item->setIcon(0, *notSelectedIcon);
        }
    } else if(upwindScene){
        for(int i = 0; i < this->upwindSceneItem->childCount(); i++){
            QTreeWidgetItem * item = this->upwindSceneItem->child(i);
            if(item->text(0) == pluginName)
                item->setIcon(0, *selectedIcon);
            else item->setIcon(0, *notSelectedIcon);
        }
    }
}

void PluginSelect::constructTreeView(){
    QSet<UWPluginInterface*> plugins = UWCore::getInstance()->getPluginManager()->getLoadedPlugins();

    QTreeWidget *treeWidget = ui->treeWidget;
    treeWidget->clear();
    treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;

    chartProviderItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(CHART_PROVIDERS_STRING)));
    rendererItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(VIEW_RENDERERS_STRING)));
    nmeaReaderItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(NMEA_READERS_STRING)));
    routeManagerItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(ROUTE_MANAGERS_STRING)));
    loggerItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(LOGGERS_STRING)));
    upwindSceneItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(UPWIND_SCENES_STRING)));
    instrumentItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(INSTRUMENTS_STRING)));

    items.append(chartProviderItem);
    items.append(rendererItem);
    items.append(nmeaReaderItem);
    items.append(routeManagerItem);
    items.append(loggerItem);
    items.append(upwindSceneItem);
    items.append(instrumentItem);

    foreach(UWPluginInterface* plugin, plugins){
        QTreeWidgetItem *pluginItem =  new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(plugin->getName())));

        ChartProviderInterface* chartProvider = dynamic_cast<ChartProviderInterface *>(plugin);
        ViewRendererInterface* viewRenderer = dynamic_cast<ViewRendererInterface *>(plugin);
        NMEAReaderInterface* nmeaReader = dynamic_cast<NMEAReaderInterface *>(plugin);
        RouteManagerInterface* routeManager = dynamic_cast<RouteManagerInterface *>(plugin);
        LoggerInterface* logger = dynamic_cast<LoggerInterface *>(plugin);
        UpWindSceneInterface* upwindScene = dynamic_cast<UpWindSceneInterface *>(plugin);
        NMEAInstrumentInterface* instrument = dynamic_cast<NMEAInstrumentInterface *>(plugin);

        if(!instrument)
            pluginItem->setIcon(0, *notSelectedIcon);

        if(chartProvider){
            if(chartProvider == UWCore::getInstance()->getPluginManager()->getChartProvider())
                pluginItem->setIcon(0, *selectedIcon);
            chartProviderItem->addChild(pluginItem);
        }
        if(viewRenderer){
            if(viewRenderer == UWCore::getInstance()->getPluginManager()->getViewRenderer())
                pluginItem->setIcon(0, *selectedIcon);
            rendererItem->addChild(pluginItem);
        }
        if(nmeaReader){
            if(nmeaReader == UWCore::getInstance()->getPluginManager()->getNMEAReader())
                pluginItem->setIcon(0,*selectedIcon);
            nmeaReaderItem->addChild(pluginItem);
        }
        if(routeManager){
            if(routeManager == UWCore::getInstance()->getPluginManager()->getRouteManager())
                pluginItem->setIcon(0, *selectedIcon);
            routeManagerItem->addChild(pluginItem);
        }
        if(logger){
            if(logger == UWCore::getInstance()->getPluginManager()->getLogger())
                pluginItem->setIcon(0,*selectedIcon);
            loggerItem->addChild(pluginItem);
        }
        if(upwindScene){
            if(upwindScene == UWCore::getInstance()->getPluginManager()->getUpWindScene())
                pluginItem->setIcon(0,*selectedIcon);
            upwindSceneItem->addChild(pluginItem);
        }
        if(instrument){
            instrumentItem->addChild(pluginItem);
        }
    }
    treeWidget->insertTopLevelItems(0, items);
}

void PluginSelect::setupUI(){
    ui->textEdit->setVisible(false);
    QSet<UWPluginInterface*> plugins = UWCore::getInstance()->getPluginManager()->getLoadedPlugins();

    this->constructTreeView();

    foreach(UWPluginInterface* plugin, plugins){
        if(dynamic_cast<ChartProviderInterface *>(plugin))
            chartProviderBox->addItem(plugin->getName());
        if(dynamic_cast<ViewRendererInterface *>(plugin))
            rendererBox->addItem(plugin->getName());
        if(dynamic_cast<NMEAReaderInterface *>(plugin))
            nmeaReaderBox->addItem(plugin->getName());
        if(dynamic_cast<RouteManagerInterface *>(plugin))
            routeManagerBox->addItem(plugin->getName());
        if(dynamic_cast<LoggerInterface *>(plugin))
            loggerBox->addItem(plugin->getName());
        if(dynamic_cast<UpWindSceneInterface *>(plugin))
            upwindSceneBox->addItem(plugin->getName());
    }

    Settings *selectedPlugins = UWCore::getInstance()->getSettingsManager()->getSelectedPlugins();

    int index = chartProviderBox->findText(selectedPlugins->getSetting("ChartProvider"));
    if(index != -1)
        chartProviderBox->setCurrentIndex(index);

    index = nmeaReaderBox->findText(selectedPlugins->getSetting("NMEAReader"));
    if(index != -1)
        nmeaReaderBox->setCurrentIndex(index);

    index = loggerBox->findText(selectedPlugins->getSetting("Logger"));
    if(index != -1)
        loggerBox->setCurrentIndex(index);

    index = routeManagerBox->findText(selectedPlugins->getSetting("RouteManager"));
    if(index != -1)
        routeManagerBox->setCurrentIndex(index);

    index = upwindSceneBox->findText(selectedPlugins->getSetting("UpWindScene"));
    if(index != -1)
        upwindSceneBox->setCurrentIndex(index);

    index = rendererBox->findText(selectedPlugins->getSetting("Renderer"));
    if(index != -1)
        rendererBox->setCurrentIndex(index);

    this->saveSettings();
}

void PluginSelect::saveSettings(){
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("NMEAReader", nmeaReaderBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("Renderer", rendererBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("Logger", loggerBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("UpWindScene", upwindSceneBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("RouteManager", routeManagerBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->setSetting("ChartProvider", chartProviderBox->currentText());
    UWCore::getInstance()->getSettingsManager()->getSelectedPlugins()->saveSettings();
}

void PluginSelect::newPluginSelected(const QString &pluginName){
    UWCore::getInstance()->getUIManager()->getSettingsForm()->setPluginSettings();
    updateTextBox();

    updateTreeView(pluginName);
    this->saveSettings();
}

void PluginSelect::on_rendererBox_textChanged(QString pluginName){}

void PluginSelect::on_nmeaReaderBox_textChanged(QString pluginName){}

void PluginSelect::on_chartProviderBox_textChanged(QString pluginName){}

void PluginSelect::on_upwindSceneBox_textChanged(QString pluginName){}

void PluginSelect::on_loggerBox_textChanged(QString pluginName){}

void PluginSelect::on_routeManagerBox_textChanged(QString pluginName){}

void PluginSelect::on_nmeaReaderBox_currentIndexChanged(QString pluginName){}

void PluginSelect::on_nmeaReaderBox_activated(QString pluginName){
    UWPluginInterface* nmeaReader = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setNMEAReader(nmeaReader);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_rendererBox_activated(QString pluginName){
    UWPluginInterface* viewRenderer = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setViewRenderer(viewRenderer);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_chartProviderBox_activated(QString pluginName){
    UWPluginInterface* chartProvider = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setChartProvider(chartProvider);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_upwindSceneBox_activated(QString pluginName){
    UWPluginInterface* upwindScene = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setUpWindScene(upwindScene);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_loggerBox_activated(QString pluginName){
    UWPluginInterface* logger = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setLogger(logger);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_routeManagerBox_activated(QString pluginName){
    UWPluginInterface* routeManager = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
    UWCore::getInstance()->getPluginManager()->setRouteManager(routeManager);
    this->newPluginSelected(pluginName);
}

void PluginSelect::on_treeWidget_itemDoubleClicked(QTreeWidgetItem* item, int column){
    QString pluginName = item->text(0);

    if(item->parent() != 0){

        if(item->parent()->text(0) == NMEA_READERS_STRING){
            nmeaReaderBox->setCurrentIndex(nmeaReaderBox->findText(pluginName));
            UWPluginInterface* nmeaReader = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setNMEAReader(nmeaReader);
            this->newPluginSelected(pluginName);
        }
        if(item->parent()->text(0) == VIEW_RENDERERS_STRING){
            rendererBox->setCurrentIndex(rendererBox->findText(pluginName));
            UWPluginInterface* viewRenderer = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setViewRenderer(viewRenderer);
            this->newPluginSelected(pluginName);
        }
        if(item->parent()->text(0) == CHART_PROVIDERS_STRING){
            chartProviderBox->setCurrentIndex(chartProviderBox->findText(pluginName));
            UWPluginInterface* chartProvider = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setChartProvider(chartProvider);
            this->newPluginSelected(pluginName);
        }
        if(item->parent()->text(0) == UPWIND_SCENES_STRING){
            upwindSceneBox->setCurrentIndex(upwindSceneBox->findText(pluginName));
            UWPluginInterface* upwindScene = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setUpWindScene(upwindScene);
            this->newPluginSelected(pluginName);
        }
        if(item->parent()->text(0) == LOGGERS_STRING){
            loggerBox->setCurrentIndex(loggerBox->findText(pluginName));
            UWPluginInterface* logger = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setLogger(logger);
            this->newPluginSelected(pluginName);
        }
        if(item->parent()->text(0) == ROUTE_MANAGERS_STRING){
            routeManagerBox->setCurrentIndex(routeManagerBox->findText(pluginName));
            UWPluginInterface* routeManager = UWCore::getInstance()->getPluginManager()->getPlugin(pluginName);
            UWCore::getInstance()->getPluginManager()->setRouteManager(routeManager);
            this->newPluginSelected(pluginName);
        }
    }
}
