#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QObject>
#include <QWidget>

#include "../../Settings/settings.h"
#include "settingsui.h"

class NMEALogger;

namespace Ui {
class SettingsUI;
}

/** Declaration of SettingsUI, the user interface of Logger's settings.
  */

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    /** Constructor of Settings user interface
      * @param logger - pointer to NMEALogger
      */
    explicit SettingsUI(NMEALogger *logger);
    ~SettingsUI();

    /** Setups Settings in the user interface depending on the settings that are applied
      * @param s - Pointer to Settings
      */
    void setupSettings(Settings* s);

    /** Updates settings file once the settings are changed in the user interface
      */
    void updateSettings();

private:
    Ui::SettingsUI *ui;
    NMEALogger *logger;
    Settings *settings;

private slots:
    void on_compassCB_toggled(bool checked);
    void on_clockCB_toggled(bool checked);
    void on_gpsCB_toggled(bool checked);
    void on_anemometerCB_toggled(bool checked);
    void on_RegularSavingRB_toggled(bool checked);
    void on_TimedSavingRB_toggled(bool checked);
    void on_RegularSavingSB_valueChanged(int);
    void on_TimedSavingSB_valueChanged(int);
};

#endif // SETTINGSUI_H



