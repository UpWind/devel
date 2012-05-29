/**
 * @file charteditorwindow.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef CHARTEDITORWINDOW_H
#define CHARTEDITORWINDOW_H

#include <QtGui/QMainWindow>
//#include "../settings/SettingsManager.h"
#include "../layers/ChartTableGen.h"
#include "generatorsettingsdialog.h"
#include "loadcharttabledialog.h"
#include "../layers/charttablesettings.h"
#include "../shared/utils.h"
#include "../layers/chart.h"
#include "../layers/LayerVisualInfo.h"
#include <QAbstractTableModel>
#include <QLabel>
#include <QTableWidgetItem>
//#include <QtGui/QDialog>

namespace Ui {
    class ChartEditorWindow;
        class LayerListModel;
        class LayerSelectionItem;
}

/**
 * A helper class to tell whether a layer is selected for generation of a new chart table.
 */
class LayerSelectionItem {
public:
        LayerSelectionItem();
        LayerSelectionItem(QString layerId, bool selected);

        QString getLayerId();
        bool getSelected();
        void setLayerId(QString layerId);
        void setSelected(bool selected);

private:
        QString layerId;
        bool selected;
};


/**
 * Chart table editor main window.
 *
 * This class can be used in a stand-alone editor as-is. If you want to embed the editor
 * into another application, you can tell for example:
 * <ul>
 *  <li>chartEditorWindow->setWindowFlags(Qt::Dialog);
 *  <li>chartEditorWindow->setWindowModality(Qt::WindowModal);
 *  <li>and optionally: chartEditorWindow->showFullScreen();
 * <ul>
 */
class ChartEditorWindow : public QMainWindow {
    Q_OBJECT
public:
    ChartEditorWindow(QWidget *parent = 0);
        ~ChartEditorWindow();

protected:
    void changeEvent(QEvent *e);
        void resizeEvent(QResizeEvent *e);
        void paintEvent(QPaintEvent *e);

private:
        SettingsManager* settingsManager;
    Ui::ChartEditorWindow *m_ui;
        QVector<LayerVisualInfo*> lvi;	// = chart table data
        ChartTableGen chartTable;			// this generates chart table data
        QString chartTableXMLFile;

        QLabel *statusMessageLabel;
        QLabel *statusModifiedLabel;	// a placeholder, and makes status bar behave and look better

        GeneratorSettingsDialog * generatorSettingsDialog;
        LoadChartTableDialog * loadChartTableDialog;

        ChartTableSettings chartTableSettings;

        bool loadChartTable();
        bool createChartTable();

        void showChartTable();
        void showLayers();
        bool clearLayerSelectionTable;
        QVector<LayerSelectionItem*> layerSelectionVector;

        // show message in status bar
        void showStatus(QString message);

public slots:
        void showGeneratorSettingsDialog();
        void changeChartTableSettings();
        void showLoadChartTableDialog();
        void onLoadChartTableAccept();
        void saveChartTable();
        void saveAsChartTable();
        void onActionGenerateTriggered();
        void onActionFetchLayersFromDBTriggered();
        void onActionIncludeAllTriggered();
        void onActionExcludeAllTriggered();
        void onActionRemoveUnusedLayersTriggered();

        void onLayerSelectionTableItemChanged(QTableWidgetItem* item);
};

#endif // CHARTEDITORWINDOW_H
