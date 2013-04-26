#include "datasimulator.h"

#include <stdlib.h>
#include <stdio.h>
#include <QMessageBox>
#include <qmath.h>
#include "../shared/uwmath.h"
#include "../shared/polardiagram.h"

const static double KNOT_TO_METER_MULTI = 0.514444444;

dataSimulator::dataSimulator() :
    m_currentGpsPositionLatitude(0)
  , m_currentGpsPositionLongitude(0)
  , m_currentCompassHeading(0.0)
  , m_currentVelocity(0.0)
  , m_polarDiagram(0)
  , m_windAngle(0.0)
  , m_windSpeed(0.0)
{
    delay = 1000;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(generate()));
    timer->start(delay);

    settingsUI = new SettingsUI();
    settingsUI->setReader(this);
    this->initializeSettings();
    settingsUI->setupSettings(settings);
    display = false;
    compass = settingsUI->getCompass();
    anonemeter = settingsUI->getAnenomter();
    clock = settingsUI->getClock();
    gps = settingsUI->getGPS();
    m_currentGpsPositionLongitude = settingsUI->boatPositionLongitude();
    m_currentGpsPositionLatitude = settingsUI->boatPositionLatitude();

    m_currentCompassHeading = 0; // clockwise degrees from north
    m_currentVelocity = 1000.4; // knots per hour, one knot
    m_currentSteeringSpeed = 0.0;

    m_windAngle = 270.0;
    m_windSpeed = 20; // 20 knots or roughly 10m/s

    m_polarDiagram = new PolarDiagram();
    m_polarDiagram->populate();
}

dataSimulator::~dataSimulator(){}

void dataSimulator::setDelay(int newDelay){
    delay = newDelay;
    timer->setInterval(newDelay);
}

void dataSimulator::run() {}

void dataSimulator::generate(){
//    if(compass || anonemeter || clock || gps){
//        bool repeat = true;
//        int value;
//        while(repeat){
//            value = randInt(0,3);§
//            switch(value){
//            case 0: if(compass) repeat = false; break;
//            case 1: if(gps) repeat = false; break;
//            case 2: if(anonemeter) repeat = false; break;
//            case 3: if(clock) repeat = false; break;
//            default: break;
//            }
//        }
//        switch(value){
//        case 0: simulateNMEACompass(); break;
//        case 1: simulateNMEAGPS(); break;
//        case 2: simulateNMEAAngleAndWindSpeed(); break;
//        case 3: simulateNMEAClock(); break;
//        default: break;
//        }
//    }
    simulateNMEACompass();
    simulateNMEAGPS();
    simulateNMEAAngleAndWindSpeed();
    simulateNMEAClock();
}

bool dataSimulator::getDisplay(){
    return display;
}

QString dataSimulator::getName(){
    return QString("DataNMEASimulator");
}

void dataSimulator::setDisplay(bool d){
    display = d;
}

void dataSimulator::simulateNMEAClock(){
    /*ZDA - Data and Time

      $GPZDA,hhmmss.ss,dd,mm,yyyy,xx,yy*CC
      $GPZDA,201530.00,04,07,2002,00,00*60

    where:
            hhmmss    HrMinSec(UTC)
            dd,mm,yyy Day,Month,Year
            xx        local zone hours -13..13
            yy        local zone minutes 0..59
            *CC       checksum*/

    QString str;
    str = "$IIZDA,";
    str += getTime() + ".00,";
    str += getParseDate();
    str += getLocalTime();
    str += "*" + generateChecksum(str);
    emit newNMEAString(str);
}

void dataSimulator::simulateNMEAAngleAndWindSpeed(){
    /**
    === MWV - Wind Speed and Angle ===

    ------------------------------------------------------------------------------
            1   2 3   4 5
            |   | |   | |
     $--MWV,x.x,a,x.x,a*hh<CR><LF>
    ------------------------------------------------------------------------------

    Field Number:

    1. Wind Angle, 0 to 360 degrees
    2. Reference, R = Relative, T = True
    3. Wind Speed
    4. Wind Speed Units, K/M/N
    5. Status, A = Data Valid
    6. Checksum
    **/

    float windSpeedMS = convertWindSpeed(m_windSpeed); // M/s Speed

    QString str;
    str = "$IIMWV,";
    str += QString::number(m_windAngle);
    str += ",T,";
    str += QString::number(m_windSpeed);
    str += ",K,";
    str += QString::number(windSpeedMS);
    str+= ",M,";
    str += "A,";
    str += "*" + generateChecksum(str);

    emit newNMEAString(str);
}

void dataSimulator::simulateNMEACompass(){
    /** $HCHDG,101.1,,,7.1,W*3C

        where:
             HCHDG    Magnetic heading, deviation, variation
             101.1    heading
             ,,       deviation (no data)
             7.1,W    variation
    **/
    qDebug() << Q_FUNC_INFO << m_currentCompassHeading << timer->interval();

    double timeDeltaSeconds = (float)timer->interval() / 1000;

    m_currentCompassHeading += timeDeltaSeconds * m_currentSteeringSpeed;
    if (m_currentCompassHeading >= 360.0)
        m_currentCompassHeading -= 360.0;
    else if (m_currentCompassHeading < 0.0)
        m_currentCompassHeading += 360.0;

    int heading = 180;
    int sim = randInt(-10,10);
    heading = heading + sim;

    QString NMEAString = "$IIHDG," + QString::number(m_currentCompassHeading, 'f', 1) + ",,,7.1,W";
    NMEAString = NMEAString + "*" + generateChecksum(NMEAString);

    emit newNMEAString(NMEAString);
}

void dataSimulator::simulateNMEAGPS(){
    /** $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A

        where:
            RMC          Recommended Minimum sentence C
            123519       Fix taken at 12:35:19 UTC
            A            Status A=active or V=Void.
            4807.038,N   Latitude 48 deg 07.038' N
            01131.000,E  Longitude 11 deg 31.000' E
            022.4        Speed over the ground in knots
            084.4        Track angle in degrees True
            230394       Date - 23rd of March 1994
            003.1,W      Magnetic Variation
            *6A          The checksum data, always begins with
    **/

    if (qFuzzyCompare(m_currentGpsPositionLatitude, 0.0)
            || qFuzzyCompare(m_currentGpsPositionLongitude, 0.0)) {
        return;
    }

    QString str = "$IIRMC,";
    str += getTime() + ",";
    str += "A,";

//    int simla = randInt(-2, 2);
//    int simlo = randInt(-2, 2);
    // antti
//    latitude += simla * 0.001; // 10km
//    longitude += simlo * 0.001;

    float trigonometricAngle = UwMath::toRadians(UwMath::toCartesian(m_currentCompassHeading));
//    if (m_currentCompassHeading > 90)
//        trigonometricAngle = (m_currentCompassHeading -90.0) / 180.0 * M_PI;
//    else
//        trigonometricAngle = (m_currentCompassHeading + 270.0) / 180.0 * M_PI;

    float twa = m_windAngle + UwMath::toCartesian(m_currentCompassHeading);
    while (twa < -180.0)
        twa += 360;
    while (twa > 180.0)
        twa -= 360;
    m_currentVelocity = (float)3600.0 / m_polarDiagram->getTA(m_windSpeed, twa) * 100;
//    m_currentVelocity = m_currentVelocity *
//            * m_currentVelocity;

//    float scaleFactor = 0.03;
    double velocityInDegrees = m_currentVelocity / 60;
    double timeDeltaHours = (float)timer->interval() / 1000 / 3600;
    double longitudeCorrection = 1 / sin(m_currentGpsPositionLatitude * M_PI / 180);

    QPointF translate(qCos( trigonometricAngle ) * timeDeltaHours * velocityInDegrees,
                      -qSin( trigonometricAngle ) * timeDeltaHours * velocityInDegrees
                      * longitudeCorrection );

    translate = UwMath::toRadians(translate);

    QPointF currentGPs(m_currentGpsPositionLongitude, m_currentGpsPositionLatitude);
    UwMath::toConformalInverted(&currentGPs);

//    UwMath::toConformalInverted(translate);

    currentGPs += translate;

    UwMath::fromConformalInverted(currentGPs);

    m_currentGpsPositionLongitude = currentGPs.x();
    m_currentGpsPositionLatitude = currentGPs.y();

    /*QPointF fixed = *//*UwMath::fromConformalInverted(translate);*/
//    QPointF fixed = translate;

//    m_currentGpsPositionLatitude += translate.x();
//    m_currentGpsPositionLongitude += translate.y();

    QString slatitude= QString::number(m_currentGpsPositionLatitude, 'f', 6);
    QString slongitude= QString::number(m_currentGpsPositionLongitude, 'f', 6);
    str += slatitude + ",";
    if(m_currentGpsPositionLatitude >= 0)
        str += "N,";
    else
        str += "S,";

    str += slongitude + ",";
    if(m_currentGpsPositionLongitude < 0)
        str += "W,";
    else
        str+= "E,";

    QString velocityString = QString::number(m_currentVelocity, 'f', 1);
    while (velocityString.length() < 5) // example "022.4"
        velocityString.prepend("0");
    str += velocityString + ",";

    QString headingString = QString::number(m_currentCompassHeading, 'f', 1);
    while (headingString.length() < 5) // example "084.4"
        headingString.prepend("0");
    str += headingString + ",";
//    str += "022.4,"; // Speed over the ground in knots
//    str += "084.4,"; //track angle
    str += getDate() + ","; //date
    str += "003.1,W"; // Magnetic Variation
    str = str + "*" + generateChecksum(str);

    emit newNMEAString(str);
}

//Settings
void dataSimulator::initializeSettings(){
    QFileInfo info(this->getName()+".xml");
    settings = new Settings(this->getName());

    if(info.exists())
        settings->loadSettings();
    else{
        //construct a new settings
        settings->setSetting("GPS", "true");
        settings->setSetting("Anenometer", "true");
        settings->setSetting("Compass", "true");
        settings->setSetting("Clock", "true");
        settings->setSetting("Timer", "600");
        // Init boat position to Oulu area
        settings->setSetting("boatPosLongitude", "25.109253");
        settings->setSetting("boatPosLatitude", "65.013026");
    }
}

void dataSimulator::setTurningSpeed(int degreesPerSecond)
{
    m_currentSteeringSpeed = degreesPerSecond;
}

dataSimulator::operator QObject *()
{
    return this;
}

void dataSimulator::addPluginSettingsToLayout(QLayout *layout){
    if(layout != 0){
        layout->addWidget(settingsUI);
    }
}

float dataSimulator::convertWindSpeed(int knots){
    int rounded; // Wind speed as an integer, with the excess decimals after the 2nd decimal place removed.
    float ms;    // Wind speed in meters per second.

    ms = knots*KNOT_TO_METER_MULTI;

    ms += 0.005; //This guarantees that the number will be rounded up, instead of down.

    rounded = ms * 100;    // This calculation may seem pointless to you, but it will remove all
    ms = rounded / 100.00; // the decimals after the 2nd decimal place.

    return ms;
}

QString dataSimulator::getTime(){
    time(&UCT);
    ptm = gmtime (&UCT);
    QString timeUCT;
    if(ptm->tm_hour < 10)
        timeUCT += "0" + QString::number(ptm->tm_hour);
    else
        timeUCT += QString::number(ptm->tm_hour);
    if(ptm->tm_min < 10)
        timeUCT += "0" + QString::number(ptm->tm_hour);
    else
        timeUCT += QString::number(ptm->tm_min);
    if(ptm->tm_sec<10)
        timeUCT+="0" + QString::number(ptm->tm_hour);
    else
        timeUCT += QString::number(ptm->tm_sec);
    return timeUCT;
}

QString dataSimulator::getDate(){
    time(&UCT);
    ptm = gmtime (&UCT);
    QString dateUCT;
    if(ptm->tm_mday < 10)
        dateUCT += "0"+QString::number(ptm->tm_mday);
    else
        dateUCT += QString::number(ptm->tm_mday);
    if(ptm->tm_mon+1 < 10)
        dateUCT += "0" + QString::number(ptm->tm_mon + 1);
    else
        dateUCT += QString::number(ptm->tm_mon + 1);
    dateUCT += QString::number((ptm->tm_year + 1900) % 100);
    return dateUCT;
}

QString dataSimulator::getParseDate(){
    time(&UCT);
    ptm = gmtime(&UCT);
    QString date;
    if(ptm->tm_mday < 10)
        date += "0" + QString::number(ptm->tm_mday) + ",";
    else
        date += QString::number(ptm->tm_mday) + ",";
    if(ptm->tm_mon + 1 < 10)
        date += "0" + QString::number(ptm->tm_mon + 1) + ",";
    else
        date+=QString::number(ptm->tm_mon + 1) + ",";
    date += QString::number(ptm->tm_year + 1900) + ",";
    return date;
}

QString dataSimulator::getLocalTime(){
    time(&UCT);
    ptm = gmtime (&UCT);
    int localHour = ptm->tm_hour;
    int localMinutes = ptm->tm_min;
    ptm = localtime(&UCT);
    QString localTime;
    localHour = ptm->tm_hour-localHour;
    localMinutes = ptm->tm_min-localMinutes;
    if(localHour < 10)
        localTime += "0" + QString::number(localHour) + ",";
    else
        localTime += QString::number(localHour) + ",";
    if(localMinutes < 10)
        localTime += "0" + QString::number(localMinutes);
    else
        localTime += QString::number(localMinutes);
    return localTime;
}

void dataSimulator::changeGPS(){
    gps = !gps;
}

void dataSimulator::changeCompass(){
    compass = !compass;
}

void dataSimulator::changeClock(){
    clock = !clock;
}

void dataSimulator::changeAnenometer(){
    anonemeter = !anonemeter;
}

void dataSimulator::changeSpeed(int speed){
    setDelay(60 * speed);
}

int dataSimulator::randInt(int low, int high){
    return qrand() % ((high + 1) - low) + low;
}

void dataSimulator::setBoatPositionLon(double longitude) {
    m_currentGpsPositionLongitude = longitude;
}

void dataSimulator::setBoatPositionLat(double latitude) {
    m_currentGpsPositionLatitude = latitude;
}

Q_EXPORT_PLUGIN2(datasimulator, dataSimulator)
