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
     * A QWidget where the chart and plugin components are drawn.
     * @return pointer to the NavigationWindow
     */
    NavigationWindow *chartDisplayWidget();

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

private slots:
    void mainWindowGeometryChanged(QRect rect);

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


