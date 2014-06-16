#ifndef NMEAVARWINDSPEEDMETER_H
#define NMEAVARWINDSPEEDMETER_H

#include "coreinterface.h"
#include "coreplugin.h"
#include <QObject>
//#include "dialog.h"
#include "mainwindow.h"


namespace Ui {
class NMEAVarWindSpeedMeter;
}

/*
 * Custom NMEA plugin.
 */

class NMEAVarWindSpeedMeter : public CorePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "fi.oulu.UpWind.PluginSimulation.NMEAVarWindSpeedMeter")
    Q_INTERFACES(CoreInterface)

private:
    Ui::NMEAVarWindSpeedMeter *ui;
    QList<double> windspeeds;
    qreal windspeed;
    bool updatewindspeed;
    bool updatewindangle;
    int userinput;
    QString wind;
QDial *updateddial;
    double knotSpeed;
    double meterSpeed;

protected:
    /**
      * Creates the GUI of the plugin.
      */
    void CreateGUI();

protected slots:
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
public slots:
    void receiveWindSpeed(qreal windspeed);
    void receiveDialValue(QDial *dial);

public:


    NMEAVarWindSpeedMeter();
    void sendDatatoPlugin(QList<double> argwindspeeds);
    virtual ~NMEAVarWindSpeedMeter();
    float convertWindSpeed(int knots);
    QString getInvertedDialValue(QDial* dial);
    bool isAutoButtonPressed();
    void check();

public:
    //Dialog *dia;
MainWindow *win;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    //void valuesReady(QList<double>);

public:
    //QList<qreal> windspeeds;
};

#endif // NMEAVARWINDSPEEDMETER_H

