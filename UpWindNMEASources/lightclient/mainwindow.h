#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "client.h"
#include "serialport.h"
#include <QTimer>
#include <QStringList>

class UpwindHighlighter;

namespace Ui
{
    class MainWindow;
}

const QString XML_FILE = "settings.xml";

/**
  * MainWindow of the lightweight client.
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

    /**
      * Adds a message to a delayed output.
      * @param line QString containing the text to be written to output.
      */
    inline void appendOutput(const QString &line);
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

public slots:
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
};

#endif // MAINWINDOW_H
