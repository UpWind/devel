#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "ui_settingswindow.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    Ui::settingsWindowDLG ui;

signals:
    void goBack();
    //void chartReloadNeeded();

public slots:
    void back();
    void showSettings();
};

#endif // SETTINGSWINDOW_H
