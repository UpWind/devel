#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"

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
    void updateSettings();

private:
    Ui::SettingsUI *ui;
    Settings *settings;

private slots:
    void on_checkBox_2_clicked();
    void on_checkBox_clicked();
};

#endif // SETTINGSUI_H
