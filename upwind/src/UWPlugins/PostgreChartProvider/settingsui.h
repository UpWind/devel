#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"
#include "postgrechartprovider.h"

class PostgreChartProvider;

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

    void setupSettings(Settings* s);

    /** Set the PostgreChartProvider vinculated with
      * this SettingsUI class
      */
    void setPGChart(PostgreChartProvider * pgcp);

signals:

public slots:
    /** Save the settings
      */
    void save();

private slots:
    void on_level_edit_textChanged(const QString &arg1);

private:
    /** Save the settings with new parameters that the user has chosen
      */
    void updateSettings();

    /** Get all the dataBases names with the upWind structure and add them to a combox
      */
    void getChartNames();

    bool testDBType(QString databaseName);

private:
    Ui::SettingsUI *ui;
    Settings *settings;
    PostgreChartProvider *pgchart;
};

#endif // SETTINGSUI_H
