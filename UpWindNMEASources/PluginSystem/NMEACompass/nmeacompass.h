#ifndef NMEACOMPASS_H
#define NMEACOMPASS_H

#include <QObject>
#include <QWidget>
#include <QFrame>
#include <QtWidgets>
#include <QTimer>
#include "../CorePlugin/coreinterface.h"
#include "../CorePlugin/coreplugin.h"

const int updateDelay = 1000;

namespace Ui {
    class CompassWidget;
}

class NMEACompass : public CorePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.PluginSimulation.NMEACompass")
    Q_INTERFACES(CoreInterface)
private:
    Ui::CompassWidget *ui;
    int heading;
    //QWindowsStyle winStyle;

    //parseNMEAStrings variables to store values
protected:
    /**
     *Creates the user interface
    */
    void CreateGUI();

protected slots:
    //interface methods
    /**
      * Implements parseNMEAString.
      * Parses an NMEAString, loading the values from it and updating the GUI
      * @param text - Reference to the QString which will be parsed
      */
    void parseNMEAString( const QString & text);

    /**
      * Implements updateNMEAString.
      * Slot is called by the timer. Calculates the new NMEA-string based on the available variables.
      */
    void updateNMEAString();

    //Dial controller methods
    /**
     * Take the value of the lineEdit (always numbers between 0-359 degrees)
     * and set the correct dial value. Dial default value is 0 on its bottom
     * and we want 0 to be on the top.
    */
    void updateDial();

    /**
     * Take the value from the dial (0-359 degrees) and convert it into
     * the correct values. Dial default value is 0 on its bottom and we want
     * 0 to be on the top.
     * @param value, value taken from the dial.
    */
    void updateLineEdit(int);
    /**
      * Tests if the plugin supports (parses) the message ID.
      * @param m_ID - Reference to the string containing the message id and nothing else.
      * @param excludeProduced - Set to true if you want to exclude the message IDs the plugin produces.
      * @return true if the plugin supports the message id (parsing of the message is supported)
      */
    virtual bool supports(const QString &m_ID, bool excludeProduced);

public:
    NMEACompass();
    virtual ~NMEACompass();
    /**
      * Used to read the heading directly from the compass.
      * @return Returns the heading
      */
    int getHeading();
    /**
      * Used to set the heading directly to the compass.
      * @param heading New heading (compass direction).
      */
    void setHeading(int heading);
    /**
      * Sets the delay of the timer
      * @param tDelay - delay of the timer in milliseconds
      */
    virtual void setTimerDelay(int tDelay);
};

#endif // NMEACOMPASS_H
