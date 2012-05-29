#ifndef BOAT_H
#define BOAT_H
#include <QDomElement>

class Boat
{
protected:
    double interpolatedBoatSpeedLow;
    double interpolateBoatSpeedHigh;
    double lowAngle;
    double highAngle;
    double lowerAngle1;
    double lowerAngle2;
    double upperAngle1;
    double upperAngle2;
    double lowerWindSpeed1;
    double lowerWindSpeed2;
    double upperWindSpeed1;
    double upperWindSpeed2;
    double lowWindSpeed;
    double highWindSpeed;
public:
    static const double EARTHRADIUS = 6366.1977;
    static const double PI = 3.141592;
    static const int updateDelay = 300;
    double latitude;
    double longnitude;
    QDomElement element;
    QDomNode tabla;
    QDomNode tablaSpinnaker;
    double boatDirection;
    double windDirection;
    double windSpeed;
    double boatSpeed;  

    bool usingSpinnaker;
    QString fileName;

    int timerMultiplier;

    bool isMoving;
    bool positionSelected;
    bool spinnakerAvailable;
    Boat();

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
     * This method returns the real boat speed, calculated with the wind speed
     * and the difference between wind and boat angles.
     * The value stored in the XML file is the boat speed in TIME ALLOWANCE (TA). This means
     * that the speed in knots is speed(kt) = 1 /(TA/3600)
     * @param windSpeed, double variable of the speed of the wind got from the anemometer.
     * @return interpolated speed of the boat.
    */
    double interpolateData(double windSpeed, double angle);

    /**
     * This method gets the necessary speed values for the wind speed that we get from
     * the anemometer plugin. This must be done because if the wind speed is not the same
     * speed than the one stored in the XML file whe have to do an interpolation. To do that
     * we need the previous and the next wind speed value stored.
     * Finally we save this values in two global variables.
     * @param windSpeed, double variable of the speed of the wind got from the anemometer.
    */
    void getWindSpeed(double windSpeed);

    /**
     * This method gets the previous and the next value of the given angle (if the
     * angle is the same as the one stored in the XML file, we take it as the "previous" one)
     * This is necessary to interpolate the internal values.
     * Finally we call an interpolate function that makes the mathematical calculation of the
     * interpolation and returns the boat speed.
     * @param windSpeed, double variable containing the speed of the wind got from the anemometer.
     * @param angle, double variable containing the difference between the boat and the wind direction.
    */
    double getAngleValues(double windSpeed, double angle);

    /**
     * Method that makes a mathematical calculation. It interpolates whatever we
     * put in its inputs. y = y0 + ((y1 - y0)/(x1 - x0))*(x - x0).
     * @param value1, low angle (windSpeed) to interpolate (x0).
     * @param value2, high angle (windSpeed) to interpolate (x1).
     * @param value3, needed angle (windSpeed) to interpolate (x).
     * @param boatSpeed1, boatSpeed for the actual data (y0).
     * @param boatSpeed2, boatSpeed for the actual data (y1).
     * @return boatSpeed, the interpolate boatSpeed (y).
    */
    double interpolate(double value1, double boatSpeed1,double value2,double boatSpeed2, double value3);

    /**
     * This method gets the 2 out of XML range angles and their TA to make a linear extrapolation.
     * @param windSpeed, double variable of the speed of the wind got from the anemometer.
     * @param angle, double variable of the difference of the angle (difference = compass - anemometer)
     * @param firstValue, value of the real index in the XML file. (e.g.: the lowest angle in the XML for a given wind speed)
     * @param secondValue, value of the real index in the XML file. (e.g.: the second highest angle in the XML for a given wind speed)
     * @return extrapolated value.
    */
    double extrapolateAngle(double windSpeed, double angle, double firstValue, double secondValue);

    /**
     * This method makes a linear extrapolation. It makes a line with 2 points and with that
     * line it calculates the boat speed for the wanted angle.
     * Y = mX + b
     * m = (Y2-Y1)/(X2-X1)
     * @param value1, angle or wind speed to make the line (X1).
     * @param value2, angle or wind speed to make the line (X2).
     * @param boatSpeed1, speed of the boat for the first angle (Y1).
     * @param boatSpeed2, speed of the boat for the second angle (Y2).
     * @param neededValue, the angle or the wind speed needed for the extrapolation (X).
     * @return extrapolate value for the previous data (Y).
    */
    double createLineExtrapolation(double value1, double value2, double boatSpeed1, double boatSpeed2, double neededValue);

    // To be called
    /**
      * Check if there is any XML file selected.
      * It takes and convert all the necesary data to get and interpolate the boat speed.
      * @param anemoSpeed, wind speed got from the anemometer.
      * @param anemoangle, wind angle got from the anemometer.
      * @param boatAngle, boat angle got from the compass.
      * @return interpolate boat Speed (final value).
      */
    double calculateBoatSpeed(double anemoSpeed, double anemoAngle, double boatAngle);

    /**
      * Get the two lower angles for a given wind speed and store them in 2 global variables.
      * @param windSpeed, wind speed got from the anemometer.
      */
    void get2LowerAngles(double windSpeed);

    /**
      * Get the two higher angles for a given wind speed and store them in 2 global variables.
      * @param windSpeed, wind speed got from the anemometer.
      */
    void get2UpperAngles(double windSpeed);

    /**
      * Get the two lower values of wind speed of the current XML file, and store them in 2
      * global variables.
      */
    void get2LowerWindSpeed();

    /**
      * Get the two higher values of wind speed of the current XML file, and store them in 2
      * global variables.
      */
    void get2UpperWindSpeed();

    /**
      * Takes the TA from the XML file for 2 diferent and consecutive wind speed and an angle and extrapolate
      * it to the given wind speed.
      * @param windSpeed, wind speed wanted to do the extrapolation.
      * @param angle, the angle from which we need the TA for each wind speed that exists in the XML file.
      * @param firstValue, first wind speed needed that exists in the XML file.
      * @param secondValue, next wind speed needed that exists in the XML file.
      * @return extrapolate value.
      */
    double extrapolateWindSpeed(double windSpeed, double angle , double firstValue, double secondValue);

    /**
     * This method gets the necessary angle values for the wind speed and angle that we get from
     * the anemometer and compass plugins. This must be done because if the wind speed is not the same
     * speed than the one stored in the XML file whe have to do an interpolation. To do that
     * we need the previous and the next wind speed value stored.
     * Finally we save this values in two global variables.
     * @param windSpeed, double variable of the speed of the wind got from the anemometer.
     * @param angle, double variable of the difference of the angle between compass and anemometer.
    */
    void getAngles(double windSpeed, double angle);

    /**
      * This method calculates the next position of the boat and set it in the simulator. It takes
      * into account the speed, the direction and the current position of the boat and calculates the
      * point where the boat is going to be the next time the timer updates its value (updateDelay).
      * @return 0 if it is working properly, -1 otherwise.
      */
    int calculateEndPoint();

    /**
      * Implement getName.
      * Returns the name of the plugin
      * @return name of the plugin
      */
    virtual void setTimerMultiplier(int multiplier);

    /**
      * This method parses the latitude in degrees, minutes and seconds to decimal value.
      * @param latstr, QString with latitude information in degrees, minutes and seconds.
      * @return double value with the same latitude but in decimal value.
      */
    static double latDecimal( const QString & latstr);

    /**
      * This method parses the longitude in degrees, minutes and seconds to decimal value.
      * @param latstr, QString with longitude information in degrees, minutes and seconds.
      * @return double value with the same longitude but in decimal value.
      */
    static double longDecimal( const QString & longstr);
};

#endif // BOAT_H
