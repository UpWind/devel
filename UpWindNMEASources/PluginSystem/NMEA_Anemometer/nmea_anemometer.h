#ifndef NMEA_ANEMOMETER_H
#define NMEA_ANEMOMETER_H

#include "../CorePlugin/coreinterface.h"
#include "../CorePlugin/coreplugin.h"
#include <QtGui>

const int updateDelay = 1000;

namespace Ui {
    class AnemometerWidget;
}

class NMEA_Anemometer : public CorePlugin
{
    Q_OBJECT
    Q_INTERFACES(CoreInterface)

public:
    NMEA_Anemometer();
    ~NMEA_Anemometer();
    /**
     * Returns an inverted wind angle dial value, such that north is at 0 degrees, and
     * south is at 180 degrees. Without this function the dial would return incorrect
     * wind angle values.
     * @param dial A pointer to the QDial object
     * @return A QString object containing the inverted dial value.
     */
    QString getInvertedDialValue(QDial* dial);
    /**
      * Converts the wind speed from knots to m/s and rounds it up to 2 decimal places.
      * @param knots The wind speed in knots.
      * @return The wind speed in meters per second, rounded up to 2 decimal places.
      */
    float convertWindSpeed(int knots);
    /**
      * Sets the delay of the timer
      * @param tDelay - delay of the timer in milliseconds
      */
    virtual void setTimerDelay(int tDelay);
protected:
    void CreateGUI();
private:
    Ui::AnemometerWidget *ui;
    double windSpeedValue;
    QWindowsStyle winStyle;


private slots:
    /**
      * This function is called when the wind angle dial's value changes.
      */
    void on_windAngle_valueChanged();
    /**
      * This function is called every time the wind speed slider's value is changed.
      */
    void on_windSpeed_valueChanged();
    /**
      * Parses an NMEA string which contains the wind information.
      * @param text A reference to the QString which has the NMEA information
      */
    void parseNMEAString(const QString & text);
    /**
      * Builds a new NMEA string and emits it, if it's changed
      */
    void updateNMEAString();
};

#endif // NMEA_ANEMOMETER_H
