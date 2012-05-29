#ifndef SETTINGSUI_H
#define SETTINGSUI_H

#include <QWidget>
#include "../../Settings/settings.h"

class dataSimulator;

namespace Ui {
class SettingsUI;
}

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsUI(QWidget *parent = 0);
    ~SettingsUI();

    /** Setup the UI settings
      * @param s - The settings from the dataSimulator Settings User Interface
      */
    void setupSettings(Settings* s);

    /** Save the settings with new parameters that the user has chosen
      */
    void updateSettings();

    /** Set the dataSimulator vinculated with this SettingsUI class
      */
    void setReader(dataSimulator* reader);

    /** Return the value of the GPS that the user has chosen
     */
    bool getGPS();

    /** Return the value of the Anemometer that the user has chosen
     */
    bool getAnenomter();

    /** Return the value of the Clock that the user has chosen
     */
    bool getClock();

    /** Return the value of the Compass that the user has chosen
      */
    bool getCompass();

private:
    Settings *settings;
    Ui::SettingsUI *ui;
    dataSimulator* reader;
    bool first;

private slots:
    /** Connect the NMEAString signal with the readData slot
       */
    void on_checkBox_Clock_clicked();
    void on_checkBox_Compass_clicked();
    void on_checkBox_Anenometer_clicked();
    void on_checkBox_GPS_clicked();
    void on_horizontalSlider_velocity_valueChanged(int value);
    void startReading();

    /** Stop showing the NMEA Strings in the Settings log
      */
    void endTest();

    /** Save the settings
      */
    void save();

    /** Show the NMEAStrings int Settings log
      */
    void readData(const QString & data);

    /** Change the interval value on the speed's label in seconds
      */
    void changeSpeed(int s);
};

#endif // SETTINGSUI_H
