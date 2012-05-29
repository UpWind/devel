#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"
#include "chartEditor/charteditorwindow.h"

class PGDataBase;

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

    /** Setup the UI settings
      * @param s - The settings from the dataSimulator Settings User Interface
      */
    void setupSettings(Settings* s);

    /** Save the settings with new parameters that the user has chosen
      */
    void updateSettings();

    /** Set the PGDataBase vinculated with this SettingsUI class
      */
    void setChart(PGDataBase* c);

private:
    bool testDBType(QString databaseName);
    Ui::SettingsUI *ui;
    Settings *settings;
    ChartEditorWindow *chartEditorWindow;
    PGDataBase* chart;

private slots:
    /** Save the settings
      */
    void save();

    /** Get all the dataBases names with the upWind structure and add them to a combox
      */
    void getChartNames();
    /** Open a editor to edit the DB and also generate the XML for it
      */
    void openChartEditor();
    /** Open a file Dialog to look for the XML file related to the actual dataBase
      */
    void browseChartTableXMLFile();
};

#endif // SETTINGSUI_H
