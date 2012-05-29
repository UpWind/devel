/**
 * @file charteditorwindow.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "charteditorwindow.h"
#include "ui_charteditorwindow.h"
#include <QLabel>

ChartEditorWindow::ChartEditorWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ChartEditorWindow)
{
    m_ui->setupUi(this);

    m_ui->splitter->setStretchFactor(0, 1);		// layer selection
    m_ui->splitter->setStretchFactor(1, 3);		// chart table
    this->setCentralWidget(m_ui->splitter);		// full screen splitter

    this->statusMessageLabel = new QLabel();
    this->statusModifiedLabel = new QLabel();
    m_ui->statusbar->addWidget(this->statusMessageLabel, 1);
    m_ui->statusbar->addWidget(this->statusModifiedLabel, 0);


    settingsManager = SettingsManager::getInstance();
    chartTableXMLFile = settingsManager->getChartTableXMLFile();
    chartTableSettings.setDefaultValues();

    QStringList tableHeaderLabels;
    tableHeaderLabels << "layerId" << "zoomMin" << "zoomMax" << "styleId" << "simpLevel" <<
                         "epsilon" << "sourceTableName";
    m_ui->tableWidget->setRowCount(0);
    m_ui->tableWidget->setColumnCount(tableHeaderLabels.size());
    m_ui->tableWidget->setHorizontalHeaderLabels(tableHeaderLabels);
    m_ui->tableWidget->setColumnWidth(3, 50);
    m_ui->tableWidget->setColumnWidth(4, 50);
    m_ui->tableWidget->setColumnWidth(6, 200);

    QStringList selectHeaderLabels;
    selectHeaderLabels << "Layer";
    m_ui->tableWidgetSelect->setRowCount(0);
    m_ui->tableWidgetSelect->setColumnCount(selectHeaderLabels.size());
    m_ui->tableWidgetSelect->setHorizontalHeaderLabels(selectHeaderLabels);

    generatorSettingsDialog = new GeneratorSettingsDialog(this);
    loadChartTableDialog = new LoadChartTableDialog(this);

    connect(m_ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(m_ui->actionGenerate, SIGNAL(triggered()), this, SLOT(onActionGenerateTriggered()));
    connect(m_ui->actionEditorSettings, SIGNAL(triggered()), this,
            SLOT(showGeneratorSettingsDialog()));
    connect(this->generatorSettingsDialog, SIGNAL(accepted()), this,
            SLOT(changeChartTableSettings()));
    connect(m_ui->actionLoad, SIGNAL(triggered()), this, SLOT(showLoadChartTableDialog()));
    connect(this->loadChartTableDialog, SIGNAL(accepted()), this, SLOT(onLoadChartTableAccept()));
    connect(m_ui->actionSave, SIGNAL(triggered()), this, SLOT(saveChartTable()));
    connect(m_ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAsChartTable()));
    connect(m_ui->actionFetchLayersFromDB, SIGNAL(triggered()), this,
            SLOT(onActionFetchLayersFromDBTriggered()));
    connect(m_ui->actionIncludeAll, SIGNAL(triggered()), this, SLOT(onActionIncludeAllTriggered()));
    connect(m_ui->actionExcludeAll, SIGNAL(triggered()), this, SLOT(onActionExcludeAllTriggered()));
    connect(m_ui->actionRemoveUnusedLayers, SIGNAL(triggered()), this,
            SLOT(onActionRemoveUnusedLayersTriggered()));
    connect(m_ui->tableWidgetSelect, SIGNAL(itemChanged(QTableWidgetItem*)), this,
            SLOT(onLayerSelectionTableItemChanged(QTableWidgetItem*)));
}

ChartEditorWindow::~ChartEditorWindow(){
    delete m_ui;
}

void ChartEditorWindow::changeEvent(QEvent *e){
    QMainWindow::changeEvent(e);
    switch(e->type()){
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ChartEditorWindow::resizeEvent(QResizeEvent *e){
    QMainWindow::resizeEvent(e);
}

void ChartEditorWindow::paintEvent(QPaintEvent *e){
    QMainWindow::paintEvent(e);
    // auto expanding column width is not working so do it like this
    int viewportWidth = m_ui->tableWidgetSelect->viewport()->width();
    m_ui->tableWidgetSelect->setColumnWidth(0, viewportWidth);
}

bool ChartEditorWindow::loadChartTable(){
    bool ok;

    this->lvi.clear();
    this->lvi << chartTable.loadChartTableFromXML(chartTableXMLFile, &ok);

    // TODO: doesn't show layer if it doesn't have level 0

    if(!ok){
        showStatus("Error loading " + chartTableXMLFile);
        return false;
    }
    showStatus("Loaded " + chartTableXMLFile);
    m_ui->actionSave->setEnabled(true);
    m_ui->actionSaveAs->setEnabled(true);

    // update selection list
    layerSelectionVector.clear();
    QVectorIterator<LayerVisualInfo*> lviIt(lvi);
    while(lviIt.hasNext()){
        LayerVisualInfo * tmpLVI = lviIt.next();
        if(tmpLVI->getSimpLevel() == 0){
            LayerSelectionItem * selItem = new LayerSelectionItem(tmpLVI->getLayerId(), true);
            layerSelectionVector << selItem;
        }
    }

    showChartTable();
    showLayers();

    return true;
}

void ChartEditorWindow::showChartTable(){
    QVectorIterator<LayerVisualInfo*> lviIt(lvi);
    QTableWidgetItem* tableItem;
    int row=0, col=0;

    m_ui->tableWidget->setRowCount(lvi.count());
    m_ui->tableWidget->clearContents();

    while(lviIt.hasNext()){
        col = 0;
        LayerVisualInfo* tmpLVI = lviIt.next();

        tableItem = new QTableWidgetItem(tmpLVI->getLayerId());
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(QString::number(tmpLVI->getZoomMin()));
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(QString::number(tmpLVI->getZoomMax()));
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(QString::number(tmpLVI->getStyleId()));
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(QString::number(tmpLVI->getSimpLevel()));
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(clearZeroes(QString::number(tmpLVI->getEpsilon(), 'f')));
        m_ui->tableWidget->setItem(row, col++, tableItem);

        tableItem = new QTableWidgetItem(tmpLVI->getSrcTableName());
        m_ui->tableWidget->setItem(row, col++, tableItem);

        row++;
    }
}

void ChartEditorWindow::showLayers(){
    QTableWidgetItem * tableItem;
    QVectorIterator<LayerSelectionItem*> layerSelectionVectorIt(layerSelectionVector);
    int row=0;

    m_ui->tableWidgetSelect->setRowCount(layerSelectionVector.size());
    m_ui->tableWidgetSelect->clearContents();

    while(layerSelectionVectorIt.hasNext()){
        LayerSelectionItem* selItem = layerSelectionVectorIt.next();
        tableItem = new QTableWidgetItem(selItem->getLayerId());
        if(selItem->getSelected())
            tableItem->setCheckState(Qt::Checked);
        else
            tableItem->setCheckState(Qt::Unchecked);

        m_ui->tableWidgetSelect->setItem(row, 0, tableItem);
        row++;
    }
}

bool ChartEditorWindow::createChartTable(){
    bool ok;
    QStringList selectedLayers;

    // create table using the currently selected layers

    for(int i=0; i < m_ui->tableWidgetSelect->rowCount(); i++){
        QTableWidgetItem *item = m_ui->tableWidgetSelect->item(0, i);
        if(item->checkState() == Qt::Checked)
            selectedLayers << item->text();
    }

    chartTable.setSettings(chartTableSettings);
    this->lvi = chartTable.generateLayerVisualInfoForAllTables(selectedLayers, &ok);
    if(!ok){
        showStatus("Error creating chart table.");
        return false;
    }

    showChartTable();
    //showLayers();	// don't update! must be selected before generation

    showStatus("Generated chart table");
    m_ui->actionSave->setEnabled(true);
    m_ui->actionSaveAs->setEnabled(true);

    return true;
}

void ChartEditorWindow::onActionGenerateTriggered(){
    generatorSettingsDialog->setChartTableSettings(this->chartTableSettings);
    int confirm = generatorSettingsDialog->exec();
    if(confirm)
        createChartTable();
    else
        showStatus("");
}

void ChartEditorWindow::showGeneratorSettingsDialog(){
    generatorSettingsDialog->setChartTableSettings(this->chartTableSettings);
    generatorSettingsDialog->exec();
}

void ChartEditorWindow::changeChartTableSettings(){
    chartTableSettings = generatorSettingsDialog->getChartTableSettings();
}

void ChartEditorWindow::showLoadChartTableDialog(){
    loadChartTableDialog->setDBTableName("");
    loadChartTableDialog->setXMLFileName(chartTableXMLFile);
    loadChartTableDialog->exec();
}

void ChartEditorWindow::onLoadChartTableAccept(){
    this->chartTableXMLFile = loadChartTableDialog->getXMLFileName();
    loadChartTable();
}

void ChartEditorWindow::saveChartTable(){
    bool ok;
    QString fileName = chartTableXMLFile;
    if(!QFile::exists(fileName))
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                tr("XML files (*.xml);;Any file (*)"));
    if(!fileName.isEmpty()){
        chartTableXMLFile = fileName;
        ok = chartTable.saveChartTableToXML(this->lvi, chartTableXMLFile);
        if(ok)
            showStatus(tr("Saved ") + chartTableXMLFile);
        else
            showStatus(tr("Error saving ") + chartTableXMLFile);
    }
}

void ChartEditorWindow::saveAsChartTable(){
    bool ok;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), chartTableXMLFile,
                                                    tr("XML files (*.xml);;Any file (*)"));
    if(!fileName.isEmpty()){
        chartTableXMLFile = fileName;
        ok = chartTable.saveChartTableToXML(this->lvi, chartTableXMLFile);
        if(ok)
            showStatus(tr("Saved ") + chartTableXMLFile);
        else
            showStatus(tr("Error saving ") + chartTableXMLFile);

    }
}

void ChartEditorWindow::onActionFetchLayersFromDBTriggered(){
    Chart chart;
    QString qSqlDriverStr;
    LayerSelectionItem* selItem;
    QString layerId;
    QStringList layerIdList;

    layerSelectionVector.clear();

    if(settingsManager->getDBDriver() == "MySQL")
        qSqlDriverStr = "QMYSQL";
    else if (settingsManager->getDBDriver() == "PostgreSQL")
        qSqlDriverStr = "QPSQL";

    chart.setOGRDriverQt(qSqlDriverStr);
    layerIdList << chart.getSimplifiedLayers();

    QStringListIterator layerIdListIt(layerIdList);
    while(layerIdListIt.hasNext()){
        layerId = layerIdListIt.next();
        selItem = new LayerSelectionItem(layerId, true);
        layerSelectionVector << selItem;
    }

    showLayers();
}

void ChartEditorWindow::onActionIncludeAllTriggered(){
    for(int i=0; i < m_ui->tableWidgetSelect->rowCount(); i++)
        m_ui->tableWidgetSelect->item(0, i)->setCheckState(Qt::Checked);
}

void ChartEditorWindow::onActionExcludeAllTriggered(){
    for(int i=0; i < m_ui->tableWidgetSelect->rowCount(); i++)
        m_ui->tableWidgetSelect->item(0, i)->setCheckState(Qt::Unchecked);

}

void ChartEditorWindow::onActionRemoveUnusedLayersTriggered(){
    QMutableVectorIterator<LayerSelectionItem*> lsIt(this->layerSelectionVector);
    LayerSelectionItem* selItem;
    while(lsIt.hasNext()){
        selItem = lsIt.next();
        if(!selItem->getSelected())
            lsIt.remove();
    }
    showLayers();
}

void ChartEditorWindow::onLayerSelectionTableItemChanged(QTableWidgetItem* item){
    LayerSelectionItem* selItem = layerSelectionVector.at(item->row());
    selItem->setSelected(item->checkState());
}

void ChartEditorWindow::showStatus(QString message){
    this->statusMessageLabel->setText(message);
}

LayerSelectionItem::LayerSelectionItem(){}

LayerSelectionItem::LayerSelectionItem(QString layerId, bool selected){
    this->layerId = layerId;
    this->selected = selected;
}


QString LayerSelectionItem::getLayerId(){
    return this->layerId;
}

bool LayerSelectionItem::getSelected(){
    return this->selected;
}

void LayerSelectionItem::setLayerId(QString layerId){
    this->layerId = layerId;
}

void LayerSelectionItem::setSelected(bool selected){
    this->selected = selected;
}
