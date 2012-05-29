/**
 * @file generatorsettingsdialog.h
 * @author Henrik Pitkala
 * @author UpWind::Performance Project 2009
 */
#ifndef GENERATORSETTINGSDIALOG_H
#define GENERATORSETTINGSDIALOG_H

#include <QtGui/QDialog>
#include "../layers/charttablesettings.h"
#include "../layers/ChartTableGen.h"
#include "../layers/LayerVisualInfo.h"
#include "../shared/utils.h"

namespace Ui {
    class GeneratorSettingsDialog;
}

class GeneratorSettingsDialog : public QDialog {
    Q_OBJECT
public:
    GeneratorSettingsDialog(QWidget *parent = 0);
    ~GeneratorSettingsDialog();
        ChartTableSettings getChartTableSettings();
        void setChartTableSettings(ChartTableSettings chartTableSettings);

protected:
    void changeEvent(QEvent *e);

public slots:
        void accept();
        void previewValues();

private:
    Ui::GeneratorSettingsDialog *m_ui;
        ChartTableSettings chartTableSettings;
        void setXMLFileName();
};

#endif // GENERATORSETTINGSDIALOG_H
