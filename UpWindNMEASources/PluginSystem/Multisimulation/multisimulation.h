#ifndef MULTISIMULATION_H
#define MULTISIMULATION_H

#include "coreinterface.h"
#include "coreplugin.h"
#include <QObject>
#include "client.h"
#include "boat.h"
#include <QWebFrame>
#include <QList>
#include "nmea_anemometer.h"

class CorePlugin;
class Server;
class QTcpSocket;
class Client;

/**
  * IDs used only for the multisimulation packets.
  */
namespace MultiSimuID{
    const int GPSLIST = PacketID::CUSTOM + 1;
}

enum SimulationStatus {NOTCONNECTED, READY, RUNNING, ENDED};

namespace Ui {
    class MultiWidget;
}

/**
  * Class used for saving and accessing extra data for each client.
  */
class ClientInfo: public QObject{
    Q_OBJECT
public:
    Boat boat;
    int ID;
    ClientInfo():ID(0){

    }
};

/*
 * Multisimulation plugin class used for simulating multiple boats on several computers.
 */

class Multisimulation : public CorePlugin
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)
private:
    Ui::MultiWidget *ui;
    NMEA_Anemometer windInstrument;

    Server *server;
    SimulationStatus simulationStatus;

    QObject q;

    /**
      * Used to add a delayed message to the output.
      * @param msg Message to be added.
      */
    void logMessage(const QString &msg);
    /**
      * Called after connection has been established. Prepares the program for new clients.
      */
    void doConnected();
    /**
      * Returns a list of connected users. Each name is separated by ';' symbol. Used to send user names
      to new clients.
      */
    QString getNameList();
    /**
      * Utility method. Gives the boat of the given client.
      * @param client Client whose boat will be returned
      * @return The boat of the client.
      */
    Boat *boatFromClient(Client *client);
    /**
      * List of boatnames (connected clients). Used in the listview.
      */
    QList<QListWidgetItem *> competitors;
    int oldWidth, oldHeight;
private slots:
    void spinnakerChanged(int newStatus);
protected:
    Boat xmlBoat;
    /**
      * Creates the GUI of the plugin.
      */
    void CreateGUI();
    /**
      * Reimplements the timer delay. Will always use the same delay.
      * @param tDelay New delay, which is discarded and not used.
      */
    void setTimerDelay(int tDelay);
protected slots:

    void setBoatXML();
    /**
      * Implements parseNMEAString.
      * Parses an NMEAString, loading the values from it and updating the GUI
      * @param text - Reference to the QString which will be parsed
      */
    void parseNMEAString(const QString & text, Client* client);
    //not used
    void parseNMEAString(const QString & text);
    /**
      * Resets the simulation.
      */
    void resetSimulation();
    /**
      * Implements updateNMEAString.
      * Slot is called by the timer. Calculates the new NMEA-string based on the available variables.
      */
    void updateNMEAString();

    /**
      * Starts the server and accepts new clients.
      */
    void startServer();
    /**
      * Starts the simulation. All boats will have to be placed first or a warning message is shown.
      */
    void startSimulation();
    /**
      * Selects a winner. The simulation will end, but clients will not be disconnected yet.
      */
    void selectWinner();
    /**
      * Fired when a client has connected.
      * @param The parameter is QTcpSocket pointer, which can be casted to
      Client pointer is necessary.
      */
    void clientConnected(QTcpSocket*);
    /**
      * Fired when a message is received from a client.
      * @param msg Message that is received
      * @param id ID if the received message
      * @param client Client who sent the message
      */
    void messageReceived(QString msg, int id, Client* client);
    /**
      * Fired when a new NMEA message comes from the wind instrument.
      * @param msg NMEA string from the wind instrument
      */
    void newWindInfo(QString msg);

    /**
      * Sets the location of the boat on the visual map.
      * @param client Client whose boat will be located on the map.
      */
    void setBoatMapLocation(Client *client);
    /**
      * Adds the plugin to the javascript environment for callback functionality.
      */
    void attachGPSPluginToPage();
    /**
      * Used to get the selected index from the listWidget.
      * @param index New index of the selected item.
      */
    void indexChanged(int index);
    /**
     * Select the XML file and check if it is a correct upWind XML file.
     * Also it takes two nodes of the XML file and store them in global variables,
     * this nodes are the boatSpeed with and without spinnaker. This is done
     * to have quicker access to the boat speed values, otherwise it has to find
     * the correct node each time it needs a boat speed.
     * @param dialog, true if a dialog will be shown, false if not
    */
    void setXMLFile(Boat *boat, bool dialog = true);

public slots:
    /**
      * Reads the boat coordinates from the map by javascript. Called from the javascript.
      */
      void getCoordsFromMap();
      /**
        * Fired when a client has disconnected.
        * @param socket QTcpSocket pointer that can be casted to a Client pointer if wanted.
        */
      void clientDisconnected(QTcpSocket *socket);

public:
    Multisimulation();
    virtual ~Multisimulation();
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
};

#endif // MULTISIMULATION_H
