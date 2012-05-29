#include "datasimulator.h"

#include <stdlib.h>
#include <stdio.h>
#include <QMessageBox>

const static double KNOT_TO_METER_MULTI = 0.514444444;

dataSimulator::dataSimulator(){
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
}

dataSimulator::~dataSimulator(){}

void dataSimulator::setDelay(int newDelay){
    delay = newDelay;
    timer->setInterval(newDelay);
}

void dataSimulator::run() {}

void dataSimulator::generate(){
    if(compass || anonemeter || clock || gps){
        bool repeat = true;
        int value;
        while(repeat){
            value = randInt(0,3);
            switch(value){
            case 0: if(compass) repeat = false; break;
            case 1: if(gps) repeat = false; break;
            case 2: if(anonemeter) repeat = false; break;
            case 3: if(clock) repeat = false; break;
            default: break;
            }
        }
        switch(value){
        case 0: simulateNMEACompass(); break;
        case 1: simulateNMEAGPS(); break;
        case 2: simulateNMEAAngleAndWindSpeed(); break;
        case 3: simulateNMEAClock(); break;
        default: break;
        }
    }
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

    int windAngle = 0;
    int windSpeed = 10; // knotSpeed
    int simAngle = randInt(-15, 15);
    windAngle += simAngle;
    int simSpeed = randInt(-3, 3);
    windSpeed += simSpeed;
    float windSpeedMS = convertWindSpeed(windSpeed); // M/s Speed

    QString str;
    str = "$IIMWV,";
    str += QString::number(windAngle);
    str += ",T,";
    str += QString::number(windSpeed);
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

    int heading = 180;
    int sim = randInt(-10,10);
    heading = heading + sim;

    QString NMEAString = "$IIHDG," + QString::number(heading) + ",,,7.1,W";
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

    QString str = "$IIRMC,";
    str += getTime() + ",";
    str += "A,";

    double latitude = 65.013026;
    double longitude = 25.109253; //Oulu coordinates
    int simla = randInt(-2, 2);
    int simlo = randInt(-2, 2);
    // antti
    latitude += simla * 0.001; // 10km
    longitude += simlo * 0.001;

    QString slatitude= QString::number(latitude);
    QString slongitude= QString::number(longitude);
    str += slatitude + ",";
    if(latitude >= 0)
        str += "N,";
    else
        str += "S,";

    str += slongitude + ",";
    if(longitude < 0)
        str += "W,";
    else
        str+= "E,";

    str += "022.4,"; // Speed over the ground in knots
    str += "084.4,"; //track angle
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
    }
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

Q_EXPORT_PLUGIN2(datasimulator, dataSimulator)
