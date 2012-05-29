#include "corelogger.h"

#include <QtGui>
#include <QLayout>
#include <QObject>
#include <QDomElement>

CoreLogger::CoreLogger(){
    QDir dir;
    QString logpath = "logs";
    if(!dir.cd(logpath))
        dir.mkdir("logs");

    const QString pluginname = "CoreLogger";

    //setting the savelogtimer
    savelogtimer = new QTimer();
    timedSavingOn = true;
    savelogtimer->setSingleShot(false);

    this->setRecordWind(true);
    this->setRecordClock(true);
    this->setRecordGPS(true);
    this->setRecordCompass(true);
    this->setTimedSavingOn(true);

    numberofEntries = 0;
    maxnumberofEntries = 0;
    timedSavingOn = true;
    nmealogfilename = "logs/NmeaLog_" + createEntryDate() + ".xml";
    nmealogname = "NmeaLog";

    xmlfile.setFileName(nmealogfilename);

    createNewNMEALog();
    setLogNodeList();
}


/**
  * Returns the name of the plugin
  * @return name of the plugin as a QString
  */
void CoreLogger::disableSavelogTimer(){
    if(savelogtimer->isActive())
        savelogtimer->stop();
    delete savelogtimer;
    timedSavingOn = false;
}

void CoreLogger::setSavingInterval(int time){
    //Time is given as minutes and setInterval takes milliseconds as a parameter.
    savelogtimer->setInterval(time * 60000);
}

int CoreLogger::getSavingInterval(){
    return savelogtimer->interval();
}

void CoreLogger::startSaveLogTimer(){
    savelogtimer->start();
    this->setTimedSavingOn(true);
}

void CoreLogger::stopSavelogTimer(){
    savelogtimer->stop();
    this->setTimedSavingOn(false);
}

bool CoreLogger::getSaveLogTimerState(){
    if(savelogtimer->isActive())
        return true;
    else
        return false;
}

QTimer* CoreLogger::getSaveLogTimer(){
    return savelogtimer;
}

bool CoreLogger::createNewNMEALog(){
    xmlfile.setFileName(nmealogfilename);

    //this function call creates a new file, no need to check if the file exists
    if(!xmlfile.open(QIODevice::WriteOnly))
        return false;

    // From here on we shape up the elements for the logfile
    QDomElement rootelement = nmealog.createElement("NMEA");
    nmealog.appendChild(rootelement);

    QDomElement compass = nmealog.createElement("Compass");
    rootelement.appendChild(compass);

    QDomElement wind = nmealog.createElement("Wind");
    rootelement.appendChild(wind);

    QDomElement gps = nmealog.createElement("GPS");
    rootelement.appendChild(gps);

    QDomElement clock = nmealog.createElement("Clock");
    rootelement.appendChild(clock);

    QTextStream ts(&xmlfile);
    nmealog.save(ts, 5);
    xmlfile.close();

    return true;
}

void CoreLogger::setLogFilePath(QString path){
    filepath.filePath(path);
}

void CoreLogger::setMaxNMEAEntries(int n){
    maxnumberofEntries = n;
}

void CoreLogger::appendNMEAString(QString string){
    NMEAEntry *nmea = new NMEAEntry();
    nmea->setDate( createEntryDate() );
    nmea->setNMEA(string);
    nmeacontainer.append(nmea);
    numberofEntries++;

    //This is triggered IF we are not saving the log in time intervals
    if (numberofEntries >= maxnumberofEntries && !timedSavingOn){
        emit nEntriesReceived();
        numberofEntries = 0;
    }
}

QString CoreLogger::createEntryDate(){
    QString month;
    QString day;
    QString date;

    QStringList da = QDate::currentDate().toString().split(" ");
    int m = QDate::currentDate().month();
    int d = QDate::currentDate().day();

    if(m < 10 || d < 10){
        if(m < 10 && d >= 10)
            date = da[3] + "0" + month.setNum(m) + day.setNum(d);
        else if(m >= 10 && d < 10)
            date = da[3] + month.setNum(m) + "0" + day.setNum(d);
        else if(m < 10 && d < 10)
            date = da[3] + "0" + month.setNum(m) + "0" + day.setNum(d);
    } else
        date = da[3] + month.setNum(m) + day.setNum(d);

    QStringList t = QTime::currentTime().toString().split(":");
    QString time = t[0] + t[1] + t[2];
    QString logging = date + "_" + time;
    return logging;
}

void CoreLogger::logNMEAEntries(){
    for(int i = 0; i < list.count(); i++){
        for(int ii = 0; ii < nmeacontainer.size(); ii++){
            QString nmea = nmeacontainer.at(ii)->getNMEA();
            QString date = nmeacontainer.at(ii)->getDate();

            if(nmea.contains("RMC") && list.at(i).nodeName() == "GPS" && this->getRecordGPS()){
                QDomElement element = list.at(i).toElement();
                QDomElement entry = nmealog.createElement("Entry");

                entry.setAttribute("date", date);
                entry.setAttribute("data", nmea);
                element.appendChild(entry);
            }
            if(nmea.contains("MWV") && list.at(i).nodeName() == "Wind" && this->getRecordWind()){
                QDomElement element = list.at(i).toElement();
                QDomElement entry = nmealog.createElement("Entry");

                entry.setAttribute("date", date);
                entry.setAttribute("data", nmea);
                element.appendChild(entry);
            }
            if(nmea.contains("HDG") && list.at(i).nodeName() == "Compass" && this->getRecordCompass()){
                QDomElement element = list.at(i).toElement();
                QDomElement entry = nmealog.createElement("Entry");

                entry.setAttribute("date", date);
                entry.setAttribute("data", nmea);
                element.appendChild(entry);
            }
            if(nmea.contains("ZDA") && list.at(i).nodeName() == "Clock" && this->getRecordClock()){
                QDomElement element = list.at(i).toElement();
                QDomElement entry = nmealog.createElement("Entry");

                entry.setAttribute("date", date);
                entry.setAttribute("data", nmea);
                element.appendChild(entry);
            }
        }
    }

    if(!xmlfile.open(QIODevice::WriteOnly|QIODevice::Truncate))
        QMessageBox::information(0, "CoreLogger", "Could not open the logfile to write NMEA-Strings into");
    QTextStream ts( &xmlfile );
    nmealog.save(ts,5);
    xmlfile.close();

    nmeacontainer.clear();
}

void CoreLogger::setLogNodeList(){
    list = nmealog.documentElement().childNodes();
}

QDomNodeList CoreLogger::getLogNodeList(){
    return list;
}

Settings* CoreLogger::getSettings(){
    return settings;
}

void CoreLogger::setRecordWind(bool checked){
    recordWind = checked;
}

bool CoreLogger::getRecordWind(){
    return recordWind;
}

void CoreLogger::setRecordGPS(bool checked){
    recordGPS = checked;
}

bool CoreLogger::getRecordGPS(){
    return recordGPS;
}

void CoreLogger::setRecordClock(bool checked){
    recordClock = checked;
}

bool CoreLogger::getRecordClock(){
    return recordClock;
}

void CoreLogger::setRecordCompass(bool checked){
    recordCompass = checked;
}

bool CoreLogger::getRecordCompass(){
    return recordCompass;
}

void CoreLogger::setTimedSavingOn(bool checked){
    timedSavingOn = checked;
}

bool CoreLogger::getTimedSavingOn(){
    return timedSavingOn;
}
