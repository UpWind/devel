#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QMainWindow>
#include "plugincontroller.h"
#include "settingsmanager.h"
#include "checkboxmodel.h"
#include <QTimer>
#include <QStringList>

class UpwindHighlighter;
class CoreInterface;
class InstrumentPanel;
class QListWidgetItem;
class QShortcut;

namespace Ui {
    class ControllerWindow;
}

//For controlling log playback
typedef enum {
    xmlDemoPlaying,
    xmlDemoPaused,
    xmlDemoStopped
} xmlPlayStatus;

class ControllerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControllerWindow(QWidget *parent = 0);
    ~ControllerWindow();

private:
    Ui::ControllerWindow *ui;
    PluginController controller;    
    CorePlugin *currentPlugin;
    QShortcut *NMEAConsole;
    UpwindHighlighter *highlighter;
    SettingsManager manager;
    xmlPlayStatus xmlStatus;
    bool xmlRecording;
    CheckBoxModel pluginTableModel;
    QTimer outputDelay;
    QStringList waitingOutput;
    InstrumentPanel *panel;

    QIcon minimizeIcon;
    QIcon maximizeIcon;

    inline void appendOutput(const QString &line);

private slots:

    void showPlugin(QModelIndex pluginListItem);
    void displayNMEAString(const QString &nmeaString);

    void toggleControllerServer(bool toggled);
    void toggleControllerSerial(bool toggled);

    void startXmlDemo();
    void stopXmlDemo();
    void loadXmlLogFiles();
    void showNMEAConsole();

    void startRecording();

    void stopPlugins();
    void startPlugins();
    void setSimulationSpeed(int speed);

    void maximizeSerial();
    void maximizeServer();
    void maximizeXml();

    void flushOutput();
signals:

};

#endif // CONTROLLERWINDOW_H
