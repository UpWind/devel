#ifndef NMEAGPS_H
#define NMEAGPS_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QTimer>
#include <QWebView>
#include <QWebFrame>
#include <QDomDocument>
#include "../CorePlugin/coreinterface.h"
#include "../CorePlugin/coreplugin.h"
#include "boat.h"

const int updateDelay = 1000;

namespace Ui
{
    class NMEAGPS;
}

class NMEAGPS : public CorePlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.PluginSimulation.NMEAGPS")
    Q_INTERFACES(CoreInterface)
private:
    Ui::NMEAGPS *ui;
    QWebFrame *frame;

    QButtonGroup *nsbutts;
    QButtonGroup *webutts;

    QObject q;

    Boat boat;

    /**
     * Allow you to select a XML file and set the file name in the simulator plugin.
     * It returns an integer that let us know how was the load of the XML file, if it is not
     * a valid XML file, not a valid upWind XMl file or not an XML file at all.
     * @param dialog, boolean value (true, the file exists)
     * @return state of the loaded file: 0 OK, 1 file can not be opened, 2 not a valid XML file, 3 not an UpWind file, 4 cancel button pressed
    */
    int openFile(bool dialog = true);

    /**
     * This method gets the information of the boat and set it in the
     * simulation plugin. Is really easy to add more info to the UI.
    */
    void getBoatInfo();
    /**
      * Sets the location of the boat in the visual map using javascript.
      * @param lat New latitude of the boat
      * @param lng New longitude of the boat
      */
    void setBoatMapLocation(double lat, double lng);


protected:
    /**
      * Creates the gui of the plugin.
      */
    void CreateGUI();
    /**
      * Reimplements setTimerDelay to always use the same timer delay.
      */
    void setTimerDelay(int tDelay);


protected slots:
    /**
      * Parses an incoming NMEA string.
      * @param text Valid NMEA string to be parsed.
      */
    void parseNMEAString( const QString & text);
    /**
      * Creates new NMEA strings. Called by the timer.
      */
    void updateNMEAString();
    /**
      * Reads the coordinates from the map using javascript.
      */
    void getCoordsFromMap();
    /**
      * Triggered when coordinates have changed.
      */
    void coordinatesChanged();
    /**
      * Toggless simulation off and on.
      */
    void toggleSimulation();
    /**
      * Sets moving of the boat on.
      */
    void startMoving();
    /**
      * Sets moving of the boat off.
      */
    void stopMoving();
    /**
      * Reads, validates and sets the coordinates of the boat. Uses the UI elements for reading the values.
      */
    void setCoordinates();
    /**
      * Validates coordinates.
      */
    bool validateCoordinates();
    /**
      * Adds the plugin to the javascript environment for callback functions.
      */
    void attachGPSPluginToPage();

    /**
     * Select the XML file and check if it is a correct upWind XML file.
     * Also it takes two nodes of the XML file and store them in global variables,
     * this nodes are the boatSpeed with and without spinnaker. This is done
     * to have quicker access to the boat speed values, otherwise it has to find
     * the correct node each time it needs a boat speed.
     * @param dialog, boolean value (true, the file exists)
    */
    void setXMLFile(bool dialog = true);

    /**
      * Fired when spinnaker has been toggled on or off.
      * @param state Indicates the current state of the checkbox.
      */
    void spinnakerChanged(int state);

public:
    NMEAGPS();
    ~NMEAGPS();
    /**
      * This method will save the plugin settings using the given settings manager.
      * @param sm - SettingsManager that implements the saving and loading of settings.
      */
    void saveTo(SettingsManager &sm);
    /**
      * This method will load the plugin settings using the given settings manager.
      * @param sm - SettingsManager that implements the saving and loading of settings.
      */
    QString loadFrom(SettingsManager &sm);    
    /**
      * Reimplements the timer multipler. Sets the multiplier of each boat to given multiplier.
      * @param multiplier The new multiplier
      */
    virtual void setTimerMultiplier(int multiplier);
    /**
      * Updates the ui of the plugin with the data of the boat.
      */
    void updateEndPointUI();

    /**
      * Disables the controls of the plugins.
      */
    void disableControls();

public slots:
    /**
      * Sets the boat direction. Used from javascript to change boat direction when clicking on the map.
      * @param bearing New direction of the boat.
      */
    void setBoatDirection(int bearing);

};

#endif // NMEAGPS_H
