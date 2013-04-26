#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QObject>
#include <QTimer>
#include "../NMEAReader/corenmeareader.h"
#include "time.h"
#include "settingsui.h"
#include "datasimulatorcontrolinterface.h"

class PolarDiagram;

/**
  The dataSimulator class simulate the NMEA Strings from 4 boat sail's instruments.
  It generates one of them periodically in a interval of time (Timer delay)
  */

class dataSimulator: public CoreNMEAReader, public DataSimulatorControlInterface
{
    Q_OBJECT
public:
    /** Create the timer used to generate the NMEAStrings, also load which
      * instrument is active
      */
    dataSimulator();
    ~dataSimulator();

    /** Generates a random NMEA Compass String ($IIHDG) and emit it
      */
    void simulateNMEACompass();

    /** Generates a random NMEA GPS String ($IIHDG) and emit it
      */
    void simulateNMEAGPS();

    /** Generates a random NMEA Anemometer String ($IIHDG) and emit it
      */
    void simulateNMEAAngleAndWindSpeed();

    /** Generates a random NMEA Clock String ($IIHDG) and emit it
      */
    void simulateNMEAClock();

    /**
      * Set the new delay for the Timer.
      * @param newDelay - new Delay in ms
      */
    void setDelay(int newDelay);

    /**
      * Set the new Display of the NMEAString, is used to show or not show the NMEA String in a log
      * @param d - new value of display
      */
    void setDisplay(bool d);

    /**
      *Get the new Display of the NMEAString, is used to show or not show the NMEA String in a log
      * @return value of display
      */
    bool getDisplay();

    /** Return the name of the plugin
      */
    QString getName();

    void run();

    /**
      * Add the SettingsUI to one layout.
      * @param layout - layout from the Core program
    */
    void addPluginSettingsToLayout(QLayout *layout);

    /** Put the settings from a XML file, if  the XML file doesn't exists create new one with the default settings
      */
    void initializeSettings();

    virtual void setTurningSpeed(int degreesPerSecond);

    virtual operator QObject*();

private slots:
    /** Choose one of the active instruments and call its simulate method to generate a NMEAString
      */
    void generate();

    /** Activate or deactivate the GPS instrument (If is off the generate function doesn't generate NMEAs for this instrument)
      */
    void changeGPS();

    /** Activate or deactivate the Compass instrument (If is off the generate function doesn't generate NMEAs for this instrument)
      */
    void changeCompass();

    /** Activate or deactivate the Clock instrument (If is off the generate function doesn't generate NMEAs for this instrument)
      */
    void changeClock();

    /** Activate or deactivate the Anomometer instrument (If is off the generate function doesn't generate NMEAs for this instrument)
      */
    void changeAnenometer();

    /**
      * Change the delay speed
      * @param s - new delay = s*60
     */
    void changeSpeed(int s);

private:
    bool display;
    time_t UCT;
    tm * ptm;
    int delay;

    float m_windAngle;
    float m_windSpeed;

    int randInt(int a, int b);

    QString getTime();
    QString getDate();
    QString getParseDate();
    QString getLocalTime();

    float convertWindSpeed(int knots);
    QTimer* timer;

    bool compass;
    bool gps;
    bool clock;
    bool anonemeter;

    //settings
    SettingsUI* settingsUI;

    double m_currentGpsPositionLongitude;
    double m_currentGpsPositionLatitude;
    double m_currentCompassHeading;
    double m_currentVelocity; // Speed over the ground in knots
    double m_currentSteeringSpeed; // Unit degrees per second


    PolarDiagram* m_polarDiagram;
    // controls

    QPointF* test;
};

#endif // DATASIMULATOR_H
