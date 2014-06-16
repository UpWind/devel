#ifndef NMEACLOCK_H
#define NMEACLOCK_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QTimer>
#include "../CorePlugin/coreinterface.h"
#include "../CorePlugin/coreplugin.h"

//Note for makers of new plugins:
//remember to edit your header, source and .pro file of each plugin

const int updateDelay = 1000;

namespace Ui {
    class ClockWidget;
}

class NMEAClock : public CorePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.PluginSimulation.NMEAClock")
    Q_INTERFACES(CoreInterface)
private:
    Ui::ClockWidget *ui;
    /*
    //layout used by the GUI widget of the plugin
    QGridLayout *plugin_layout;

    //all GUI elements of the plugin
    QDateTimeEdit *dateTimeEdit;
    QLabel *timeLabel;
    QLineEdit *timeZoneHEdit;
    QLineEdit *timeZoneMEdit;
    QLabel *hLabel;
    QLabel *mLabel;
    QPushButton *timeButton;
*/
    //UTC date time..
    QDateTime dateTime;

    //.. and the rest of the variables for storing the values
    int timeZoneH;
    int timeZoneM;
    QString timeZoneString;

    //returns hours in the NMEA format
    /**
      * Will return the hours in proper NMEA format.
      * Used for initializing current datetime.
      * @param hours
      * @return NMEA formatted QString containing hours
      */
    QString formatTimeZoneH(int hours);
    /**
      * Will return the minutes in proper NMEA format.
      * Used for initializing current datetime.
      * @param hours
      * @param minutes
      * @return NMEA formatted QString containing minutes
      */
    //returns minutes in the NMEA format, based on time zone
    QString formatTimeZoneM(int hours, int minutes);
    //used for updating the label when the timer is stopped, does not create new NMEA strings
    QTimer labelTimer;
protected:
    /**
      * Creates the GUI of the plugin.
      */
    void CreateGUI();
protected slots:
    /**
      * Triggered when new datetime is set in the UI. Loads values to memory.
      */
    void dateTimeChanged();
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
    /**
      * Called by the labelTimer. Updates the current datetime.
      */
    void updateLabel();
public:
    NMEAClock();
    virtual ~NMEAClock();
    /**
      * Sets the delay of the timer
      * @param tDelay - delay of the timer in milliseconds
      */
    virtual void setTimerDelay(int tDelay);
};

#endif // NMEACLOCK_H
