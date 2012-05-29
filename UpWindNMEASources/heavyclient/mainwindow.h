#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "client.h"
#include "serialport.h"
#include <QTimer>
#include <QStringList>
#include <QList>
#include "plugincontroller.h"
#include "nmeacompass.h"

class UpwindHighlighter;

namespace Ui
{
    class MainWindow;
}

/**
  * IDs used only for the multisimulation packets.
  */
namespace MultiSimuID{
    const int GPSLIST = PacketID::CUSTOM + 1;
}

const QString XML_FILE = "heavysettings.xml";

/**
  * Access class used for storing information of all map objects.
  */
class MapObject: public QObject{
    Q_OBJECT
public:
    int ID;
    QString name;
    double latitude;
    double longnitude;
    bool localBoat(const QString &localName){
        return name == localName;
    }

    MapObject(const QString& boatName, const int boatID): ID(boatID), name(boatName), latitude(0.0), longnitude(0.0){

    }
};

/**
  * MainWindow class that is the mainwindow of the client.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialPort *serialport;
    Client client;
    UpwindHighlighter *highlighter;
    QTimer outputDelay;
    QStringList waitingOutput;
    PluginController controller;
    NMEACompass compassInstrument;

    QList<MapObject*> boatList;
    int nextID;

    QObject q;

    int windDirection;

    /**
      * Adds a message to a delayed output.
      * @param line QString containing the text to be written to output.
      */
    inline void appendOutput(const QString &line);
    /**
      * Adds the MainWindow to the javascript environment for javascript to call the window functions.
      */
    void attachGPSPluginToPage();

    /**
      * Creates a new boat with the given name and adds it to the boat list. The boats are instances of MapObject class.
      */
    void addBoat(const QString &boatName);
    /**
      * Loads the client settings from an xml file.
      * @param fileName Filename of the xml file to be read.
      */
    void loadSettings(const QString& fileName);
    /**
      * Saves the client settings to an xml file.
      * @param fileName Filename of the xml file to be written into.
      */
    void saveSettings(const QString& fileName);
private slots:
        /**
          * Writes the delayed output immediately to the screen.
          */
        void flushOutput();
        /**
          * Receives the compass instrument NMEA strings and sends them to the server.
          * @param msg NMEA message string.
          */
        void compassInfo(const QString &msg);

public slots:
        /**
          * Sets the location for a boat.
          * @param boatName Name of the boat, whose position will be set
          * @param latitude The new latitude of the boat
          * @param longitude The new longitude of the boat
          */
        void setBoatMapLocation(const QString &boatName, const QString &latitude, const QString &longitude);
        /**
          * Disconnects the client from the server.
          */
        void breakConnection();
        /**
          * Tries to connect to the server. A timeout warning will be shown if connection can't be initialized.
          */
        void makeConnection();
        /**
          * Handles all incoming data from the server. Client parameter will always be the client itself.
          * @param msg The received message.
          * @param id ID of the received message, used to identify what to do with the message.
          * @param client The client itself. Can be used instead of the MainWindow's client if wanted.
          */
        void messageArrived(const QString &msg, int id, Client *client);
        /**
          * Triggered when a client disconnects. Resets the client after disconnection.
          */
	void disconnected();
        /**
          * Tries to open or close a serial port. Serial port isn't used for anything else than the wind information so far.
          * @param toggled If true, will try to open the serial port. Otherwise tries to close it.
          */
        void openSerial(bool toggled);
        /**
          * Sets the boat direction. Used from javascript to change boat direction when clicking on the map.
          * @param bearing New direction of the boat.
          */
        void setBoatDirection(int bearing);
};

#endif // MAINWINDOW_H
