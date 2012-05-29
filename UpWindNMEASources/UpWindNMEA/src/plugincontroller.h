#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <QtCore>

#include "delaytimer.h"
#include "coreplugin.h"
#include "server.h"
#include "coreinterface.h"
#include "filecontroller.h"
#include "xmlcontroller.h"
#include "serialsrc/serialport.h"
#include "environment.h"

/**
  * PluginController is used to handle loading plugins, saving / loading XML logs and to
  * output plugin related data to network clients, serial ports and XML files.
  */

class PluginController: public QObject {

    Q_OBJECT

public:

    PluginController();
    ~PluginController();

    /**
      * Getter method for server.
      * @return Pointer to the server.
      */
    Server *getServer();

    /**
      * Returns a pointer to a list with the names of all the loaded plugins.
      * @return A list of names for all the loaded plugins.
      */
    QList<QString> *getPluginNames();
    /**
      * Returns a pointer to the plugin with the same name as the parameter.
      * @param pluginName Name of the plugin
      * @param The plugin.
      */
    CorePlugin *getPlugin(const QString &pluginName);

    /**
      * Starts the network server.
      */
    void startServer(unsigned int portNumber = 9999);
    /**
      * Stops the network server.
      */
    void stopServer();

    /**
      * Opens the serial port for serial communications.
      * @param portNumber Number of the COM port (default COM3).
      * @param baudRate Serial port baud rate.
      */
    void startSerial(unsigned int portNumber = 2, unsigned int baudRate = 4800);
    /**
      * Closes the serial port.
      */
    void stopSerial();

    /**
      * Returns true if the serial port is open.
      */
    bool isSerialOpen();
    /**
      * Saves all the gathered XML data into a file.
      */
    void saveXml();
    /**
      * Outputs the NMEA string to the server.
      * @param nmeaString The NMEA string.
      */
    void broadcastToServer(const QString &nmeaString);
    /**
      * Outputs the NMEA string to the serial port.
      * @param nmeaString The NMEA string.
      */
    void broadcastToSerial(const QString &nmeaString);
    /**
      * Outputs the NMEA string to the XML logger.
      * @param nmeaString The NMEA string.
      */
    void broadcastToXml(const QString &nmeaString);

    /**
      * Loads NMEA XML log files from the log directory.
      */
    void loadXmlLogFiles();

    /**
      * Returns the list with all the loaded XML log names;
      * @param A list with all the log names.
      */
    QList<QString> *getXmlLogFiles();

    /**
      * Loads an NMEA XML log and starts playback. Emits the newXmlLogString
      * signal when a new NMEA string is played from the log.
      * @param xmlFileName Name of the log file
      */
    void startXmlDemo(const QString &xmlFileName);

    /**
      * Stops the current NMEA XML log playback.
      */
    void stopXmlDemo();
    /**
      * Pauses the current NMEA XML log playback.
      */
    void pauseXmlDemo();
    /**
      * Resumes the paused NMEA XML log playback.
      */
    void resumeXmlDemo();
    /**
      * Loads all the available plugins from the plugin directory.
      * @param plugDir The directory where the plugins are found.
      * @param filter A string list containing filter strings. For example, a QStringList containing
      *               a "*.dll" string will cause the function to only load files with the .dll file extension.
      */
    void loadPlugins(const QString &plugDir = Environment::getPluginDirectory(),
                     const QStringList &filter = QStringList("*.dll"));

private:

    QList<QString> pluginNames;
    QMap<QString, CorePlugin *> plugins;

    Server *server;
    SerialPort *serialPort;
    XMLController *xmlController;

    QList<QString> xmlLogs;
    QList<QPair<QString, int> > *xmlLog;

    QTime xmlSimulationStartTime;
    QTime time;
    DelayTimer delayTimer;
    int offsetAmount;

    static int logIndex;

signals:
    /**
      * Emitted when the controller receives an NMEA string from one the plugins.
      * @param nmeaString The NMEA string from the plugin.
      */
    void newPluginString(const QString &nmeaString);
    /**
      * Emitted when the controller reads a timestamped NMEA string from an XML log.
      * @param nmeaString The NMEA string from the log.
      */
    void newXmlLogString(const QString &nmeaString);
    /**
      * Emits the current playback progress percentage.
      * @param percentange Playback percentage.
      */
    void xmlLogProgress(int percentage);
    /**
      * Emitted when a new NMEA XML log is saved.
      */
    void newXmlLogFile();
    /**
      * Emitted when a NMEA XML log playback has finished.
      */
    void xmlLogEnd();

private slots:

    void pluginHackFade();
    void getPluginString(const QString &nmeaString);
    void getXmlLogString();
};

#endif // PLUGINCONTROLLER_H
