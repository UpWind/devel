#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QtGui>
#include "UI/mainmenu.h"
#include "UI/settingsform.h"
#include "UI/ToolBox/toolbox.h"
#include "UI/mainwindow.h"
#include "UI/navigationwindow.h"

class UIManager : public QObject
{
    Q_OBJECT
public:

    /**
     * Constructor of the UIManager.
     */
    explicit UIManager();

    /**
     * Destructor of the UIManager.
     */
    ~UIManager();

    /**
     * Getter for the ChartWidget where the map is to be drawn.
     * @return pointer to the ChartWidget.
     */
    NavigationWindow * getMap();

    /**
     * Getter for the SettingsForm ui object.
     * @return pointer to the SettingsForm.
     */
    SettingsForm* getSettingsForm();

private:

    /**
     * Connects the instruments to the user interface.
     */
    void connectInstruments();
    MainMenu *mainMenu;
    SettingsForm *settingsWindow;
    MainWindow *mainWindow;
    NavigationWindow *navigationWindow;
    ToolBox *toolbox;

public slots:

    /**
     * Shows the settings window.
     */
    void showSettingsWindow();

    /**
     * Shows the chart window.
     */
    void showChartWindow();

    /**
     * Shows the main menu.
     */
    void showMainMenu();

    /**
     * Closes the main window.
     */
    void close();

    /**
     * Toggles toolbox visibility on/off.
     */
    void toggleToolbox();
    ToolBox *getToolBox();
};

#endif // UIMANAGER_H


