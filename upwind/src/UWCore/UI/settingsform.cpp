#include "settingsform.h"
#include "ui_settingsform.h"
#include "../uwcore.h"

#include "ChartProviderInterface/chartproviderinterface.h"
#include "ViewRenderer/viewrendererinterface.h"
#include "NMEAReader/nmeareaderinterface.h"
#include "UWPluginInterface/uwplugininterface.h"
#include "UpWindScene/UpWindSceneInterface.h"
#include "pluginselect.h"

#include <QHBoxLayout>

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    this->setParent(parent);
    ui->setupUi(this);

    tabWidget = ui->tabWidget;
    PluginSelect *pluginSelect = new PluginSelect();
    tabWidget->addTab(pluginSelect, "Select Plugins");

    setPluginSettings();
}

SettingsForm::~SettingsForm(){
    delete ui;
}

void SettingsForm::setPluginSettings(){
    while(tabWidget->count() > 1)
        tabWidget->removeTab(1);

    ChartProviderInterface* chartProvider = UWCore::getInstance()->getPluginManager()->getChartProvider();
    ViewRendererInterface* viewRenderer = UWCore::getInstance()->getPluginManager()->getViewRenderer();
    NMEAReaderInterface* nmeaReader = UWCore::getInstance()->getPluginManager()->getNMEAReader();
    UpWindSceneInterface* upWindScene = UWCore::getInstance()->getPluginManager()->getUpWindScene();
    RouteManagerInterface* routeManager = UWCore::getInstance()->getPluginManager()->getRouteManager();
    LoggerInterface* logger = UWCore::getInstance()->getPluginManager()->getLogger();

    if(chartProvider){
        chartProviderWidget = new QWidget();
        chartProviderWidget->setLayout(new QHBoxLayout);
        chartProvider->addPluginSettingsToLayout(chartProviderWidget->layout());
        tabWidget->addTab(chartProviderWidget,chartProvider->getName());
    }
    if(viewRenderer){
        viewRendererWidget = new QWidget();
        viewRendererWidget->setLayout(new QHBoxLayout);
        viewRenderer->addPluginSettingsToLayout(viewRendererWidget->layout());
        tabWidget->addTab(viewRendererWidget, viewRenderer->getName());
    }
    if(nmeaReader){
        nmeaReaderWidget = new QWidget();
        nmeaReaderWidget->setLayout(new QHBoxLayout);
        nmeaReader->addPluginSettingsToLayout(nmeaReaderWidget->layout());
        tabWidget->addTab(nmeaReaderWidget, nmeaReader->getName());
    }
    if(upWindScene){
        upWindSceneWidget = new QWidget();
        upWindSceneWidget->setLayout(new QHBoxLayout);
        upWindScene->addPluginSettingsToLayout(upWindSceneWidget->layout());
        tabWidget->addTab(upWindSceneWidget, upWindScene->getName());
    }
    if(routeManager){
        routeManagerWidget = new QWidget();
        routeManagerWidget->setLayout(new QHBoxLayout);
        routeManager->addPluginSettingsToLayout(routeManagerWidget->layout());
        tabWidget->addTab(routeManagerWidget, routeManager->getName());
    }
    if(logger){
        loggerWidget = new QWidget();
        loggerWidget->setLayout(new QHBoxLayout);
        logger->addPluginSettingsToLayout(loggerWidget->layout());
        tabWidget->addTab(loggerWidget, logger->getName());
    }
}

void SettingsForm::showSettings(){
    this->show();
}

void SettingsForm::back(){
    this->hide();
    emit goBack();
}

void SettingsForm::on_pushButton_clicked(){
    SettingsForm::back();
}
