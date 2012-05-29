#ifndef INSTRUMENTPANEL_H
#define INSTRUMENTPANEL_H

#include <QWidget>
#include <QIcon>

namespace Ui {
    class InstrumentPanel;
}

class InstrumentPanel : public QWidget {

    Q_OBJECT

public:

    explicit InstrumentPanel(QWidget *parent = 0);
    ~InstrumentPanel();

    /**
      * Parses the passed NMEA string and displays the parsed values
      * in the graphical instrument panel.
      * @param text The NMEA string
      */
    void parseNmeaString( const QString & text);

private:
    /**
      * This function calculates the true wind speed with the apparent wind information and the speed of the boat.
      * @param boatSpeed, speed of the boat in knots.
      * @param apparentWindSpeed, speed of the wind taken from the anemometer in knots.
      * @param apparentWindDirection, direction of the wind taken from the anemometer.
      * @param boatDirection, direction of the boat taken from the compass.
      * @return true wind speed in knots.
      */
    double getTrueWindSpeed(double boatSpeed, double apparentWindSpeed, double apparentWindDirection, double boatDirection);

    /**
      * This function calculates the true wind angle with the apparent wind information and the speed of the boat.
      * @param boatSpeed, speed of the boat in knots.
      * @param apparentWindSpeed, speed of the wind taken from the anemometer in knots.
      * @param apparentWindDirection, direction of the wind taken from the anemometer.
      * @param boatDirection, direction of the boat taken from the compass.
      * @return true wind angle.
      */
    double getTrueWindAngle(double boatSpeed, double apparentWindSpeed, double apparentWindDirection, double boatDirection);

    Ui::InstrumentPanel *ui;
    QIcon min;
    QIcon max;
    double trueWindAngle;
    double trueWindSpeed;
    double appWindAngle;
    double appWindSpeed;
    double boatSpeed;
    double boatAngle;

private slots:
    void hidePanel();

};

#endif // INSTRUMENTPANEL_H
