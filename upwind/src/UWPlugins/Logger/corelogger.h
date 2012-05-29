#ifndef CORELOGGER_H
#define CORELOGGER_H

#include <QObject>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QTimer>

#include "LoggerInterface.h"
#include "nmeaentry.h"
#include "../../Settings/settings.h"

/** Class definition of class CoreLogger. Inherits QObject and implements LoggerInterface
  */
class CoreLogger: public QObject, public LoggerInterface
{
    Q_OBJECT
    Q_INTERFACES(LoggerInterface UWPluginInterface)

public:
    CoreLogger();

    /**
      * Creates a new NMEALogfile with initial elements for various types of NMEA strings CoreLogger may receive
      * @return boolean value according to whether file creation was successfull
      */
    bool createNewNMEALog();

    /**
      * Creates a string that represents the time and date on which an entry was made on the logfile
      * @return QString containing date and time
      */
    QString createEntryDate();

    /**
      * Sets a time interval as minutes. After each timeout logger saves NMEA strings into a logfile.
      * @param time - integer which is the interval as minutes.
      */
    void setSavingInterval(int time);

    /**
      * Gets a time interval as minutes. After each timeout logger saves NMEA strings into a logfile.
      * @return int - interval as minutes.
      */
    int getSavingInterval();

    /**
      * Sets a path to a file where logging is done to, lets say C:/upwind/log/log.xml
      * @param path
      */
    void setLogFilePath(QString path);

    /**
      * Gets a path to a file where logging is done to, lets say C:/upwind/log/log.xml
      * @return QDir variable, containing the filepath to the xmlfile.
      */
    QDir getLogFilePath();

    /**
      * Sets a QDomNodeList that contains the structure of the logfile
      */
    void setLogNodeList();

    /**
      * Gets a QDomNodeList which contains the structure of the logfile.
      *@return QDomNodeList
      */
    QDomNodeList getLogNodeList();

    /**
      * Starts the timer for saving logs.
      */
    void startSaveLogTimer();

    /**
      * Stops and resets the timer for saving logs.
      */
    void stopSavelogTimer();

    /**
      * Disables SavelogTimer and destroys the QTimer object. Called upon clean up before unpluging CoreLogger
      */
    void disableSavelogTimer();

    /**
      * Appends a NMEAString from serialport/datareader into the container
      */
    void appendNMEAString(QString);

    /**
      * Sets the maximum number of NMEA Entries plugin takes in before saving them
      * @param n - which is the max number of entries
      */
    void setMaxNMEAEntries(int n);

    /**
      * Gets the state of SaveLogTimer as boolean
      * @return bool if timer is active or not.
      */
    bool getSaveLogTimerState();

    /**
      * Initializes plugin settings by making new default settings if setting file is missing or loading the
      * current ones from a xml-file.
      */
    void initializeSettings();

    /**
      * Loads settings of plugin from a xml-file.
      */
    void loadSettings();

    /**
      * Gets Settings object, containing settings as key - value pairs.
      * @return pointer to Settings object.
      */
    Settings* getSettings();

    /**
      * Saves the settings to a file.
      */
    void saveSettings();

    /**
      * Sets boolean value according to if Wind speed and direction recording is checked
      */
    void setRecordWind(bool);

    /**
      * Gets boolean value according to if Wind speed and direction recording is checked
      */
    bool getRecordWind();

    /**
      * Sets boolean value according to if GPS recording is checked
      */
    void setRecordGPS(bool);

    /**
      * Gets boolean value according to if GPS recording is checked
      */
    bool getRecordGPS();

    /**
      * Sets boolean value according to if Clock recording is checked
      */
    void setRecordClock(bool);

    /**
      * Gets boolean value according to if Clock recording is checked
      */
    bool getRecordClock();

    /**
      * Sets boolean value according to if Compass recording is checked
      */
    void setRecordCompass(bool);

    /**
      * Gets boolean value according to if Compass recording is checked
      */
    bool getRecordCompass();

    /**
      * Sets boolean value according to if Timed Saving is checked
      */
    void setTimedSavingOn(bool);

    /**
      * Gets boolean value according to if Timed Saving is checked
      */
    bool getTimedSavingOn();

    /**
      Returns a pointer to SaveLogTimer
      @return Qtimer*
      */
    QTimer* getSaveLogTimer();


protected slots:
    /**
      * Logs contents of QStringList into a xmlfile and cleans up StringList for new entries.
      * Where in the xmlfile strings are placed, is decided by comparing the strings and the tagnames of nodes in
      * QDomNodeList.
      * This will be called every time SaveLogTimer times out or when a given number of NMEA-Strings have been appended.
      */
    void logNMEAEntries();

signals:
    /** Signal evoked whenever a NMEA-String is received
      */
    void nEntriesReceived();

private:
    int numberofEntries, maxnumberofEntries;
    QDomNodeList list;
    QString nmealogfilename;
    QString nmealogname;
    QFile xmlfile;
    QDomDocument nmealog;

    /* Stores possibly an unlimited/limited number of NMEAEntry objects, out of which the NMEA data and
       date is logged, after which the container is cleaned up and takes in other NMEA strings.
    */
    QList<NMEAEntry*> nmeacontainer;
    QDir filepath;
    QTimer *savelogtimer;

protected:
    /*SETTING VARIABLES*/

    /** Boolean indicating if NMEA-strings concerning windspeed are recorded
      */
    bool recordWind;

    /** Boolean indicating if NMEA-strings concerning GPS-coordinates are recorded
      */
    bool recordGPS;

    /** Boolean indicating if NMEA-strings concerning current time are recorded
      */
    bool recordClock;

    /** Boolean indicating if NMEA-strings concerning compass heading are recorded
      */
    bool recordCompass;

    /** Boolean indicating if NMEA-strings are recorded in timed intervals
      */
    bool timedSavingOn;

    /** Pointer to Settings class instance
      */
    Settings *settings;
};

#endif // CORELOGGER_H
