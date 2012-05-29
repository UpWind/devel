/**
 * @file generatorsettingsdialog.cpp
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#include "generatorsettingsdialog.h"
#include "ui_generatorsettingsdialog.h"

GeneratorSettingsDialog::GeneratorSettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::GeneratorSettingsDialog)
{
        m_ui->setupUi(this);
        connect(m_ui->actionOK, SIGNAL(clicked()), this, SLOT(accept()));
        connect(m_ui->actionCancel, SIGNAL(clicked()), this, SLOT(reject()));
        connect(m_ui->actionPreview, SIGNAL(clicked()), this, SLOT(previewValues()));
}

GeneratorSettingsDialog::~GeneratorSettingsDialog()
{
    delete m_ui;
}

void GeneratorSettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

ChartTableSettings GeneratorSettingsDialog::getChartTableSettings() {
        return this->chartTableSettings;
}

void GeneratorSettingsDialog::setChartTableSettings(ChartTableSettings chartTableSettings) {
        this->chartTableSettings = chartTableSettings;
//	m_ui->DBTableName->setText(chartTableSettings.chartTableDBName());
//	m_ui->fileNameLineEdit->setText(chartTableSettings.chartTableXMLFile());
        m_ui->originalZoomSpinBox->setValue(chartTableSettings.originalZoomLevel());
        m_ui->zoomScaleFactorDoubleSpinBox->setValue(chartTableSettings.scaleFactor());
        m_ui->simplificationLevelsSpinBox->setValue(chartTableSettings.simpLevelCount());
        m_ui->minimumZoomLevelSpinBox->setValue(chartTableSettings.minZoom());
        m_ui->minZoomEpsilonDoubleSpinBox->setValue(chartTableSettings.minZoomEpsilon());
        m_ui->maximumZoomLevelSpinBox->setValue(chartTableSettings.maxZoom());
        m_ui->maxZoomEpsilonDoubleSpinBox->setValue(chartTableSettings.maxZoomEpsilon());
}

void GeneratorSettingsDialog::accept() {
//	chartTableSettings.setChartTableDBName(m_ui->DBTableName->text());
//	chartTableSettings.setChartTableXMLFile(m_ui->fileNameLineEdit->text());
        chartTableSettings.setOriginalZoomLevel(m_ui->originalZoomSpinBox->value());
        chartTableSettings.setScaleFactor(m_ui->zoomScaleFactorDoubleSpinBox->value());
        chartTableSettings.setSimpLevelCount(m_ui->simplificationLevelsSpinBox->value());
        chartTableSettings.setMinZoom(m_ui->minimumZoomLevelSpinBox->value());
        chartTableSettings.setMinZoomEpsilon(m_ui->minZoomEpsilonDoubleSpinBox->value());
        chartTableSettings.setMaxZoom(m_ui->maximumZoomLevelSpinBox->value());
        chartTableSettings.setMaxZoomEpsilon(m_ui->maxZoomEpsilonDoubleSpinBox->value());
        QDialog::accept();
}

void GeneratorSettingsDialog::previewValues() {
        ChartTableSettings tmpSettings;
        ChartTableGen tmpTable;
        QVector<LayerVisualInfo*> tmpLVI;
        LayerVisualInfo* lvi;
        QTableWidgetItem* tableItem;
        int row = 0;
        int col = 0;

//	tmpSettings.setChartTableDBName(m_ui->DBTableName->text());
//	tmpSettings.setChartTableXMLFile(m_ui->fileNameLineEdit->text());
        tmpSettings.setOriginalZoomLevel(m_ui->originalZoomSpinBox->value());
        tmpSettings.setScaleFactor(m_ui->zoomScaleFactorDoubleSpinBox->value());
        tmpSettings.setSimpLevelCount(m_ui->simplificationLevelsSpinBox->value());
        tmpSettings.setMinZoom(m_ui->minimumZoomLevelSpinBox->value());
        tmpSettings.setMinZoomEpsilon(m_ui->minZoomEpsilonDoubleSpinBox->value());
        tmpSettings.setMaxZoom(m_ui->maximumZoomLevelSpinBox->value());
        tmpSettings.setMaxZoomEpsilon(m_ui->maxZoomEpsilonDoubleSpinBox->value());

        //qDebug() << tmpSettings.toString();

        tmpTable.setSettings(tmpSettings);
        tmpLVI = tmpTable.generateLayerVisualInfoForLayer("dummy");

        while (m_ui->previewTableWidget->rowCount() > 0) {
                m_ui->previewTableWidget->removeRow(0);
        }

        QVectorIterator<LayerVisualInfo*> lviIt(tmpLVI);
        while (lviIt.hasNext()) {
                if (m_ui->previewTableWidget->rowCount() <= row) {
                        m_ui->previewTableWidget->insertRow(row);
                }
                col = 0;
                lvi = lviIt.next();
                tableItem = new QTableWidgetItem(QString::number(lvi->getSimpLevel()));
                m_ui->previewTableWidget->setItem(row, col++, tableItem);

                tableItem = new QTableWidgetItem(QString::number(lvi->getZoomMin()));
                m_ui->previewTableWidget->setItem(row, col++, tableItem);

                tableItem = new QTableWidgetItem(QString::number(lvi->getZoomMax()));
                m_ui->previewTableWidget->setItem(row, col++, tableItem);

                tableItem = new QTableWidgetItem(clearZeroes(QString::number(lvi->getEpsilon(), 'f')));
                m_ui->previewTableWidget->setItem(row, col++, tableItem);

                row++;
        }
}
