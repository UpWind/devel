#include "nmeaclock.h"
#include "time.h"
#include "ui_clockwidget.h"

//implement this in each plugin
NMEAClock::NMEAClock():
        ui(new Ui::ClockWidget)
{
    dateTime = QDateTime::currentDateTime();
    timeZoneH = dateTime.time().hour() - dateTime.toUTC().time().hour();
    timeZoneM = dateTime.time().minute() - dateTime.toUTC().time().minute();
    tzset();
    if(daylight)
        timeZoneH--;
    timeZoneString = formatTimeZoneH(timeZoneH) + "," + formatTimeZoneM(timeZoneH, timeZoneM);
    dateTime = dateTime.toUTC();

    this->pluginName = "NMEA Clock";
    this->produceIDs = "ZDA";
    this->parseIDs = "ZDA";

    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName);
    //connect(&labelTimer, SIGNAL(timeout()), this, SLOT(updateLabel()));
    //start(); //don't start by default, start when user changes the UI or demo recording is started
}

NMEAClock::~NMEAClock(){
    delete ui;
}

//implement this in each plugin
void NMEAClock::CreateGUI(){
    ui->setupUi(plugin_widget);
    connect(ui->timeButton, SIGNAL(clicked()), this, SLOT(dateTimeChanged()));
    ui->timeZoneHEdit->setText(formatTimeZoneH(timeZoneH));
    ui->timeZoneMEdit->setText(formatTimeZoneM(timeZoneH, timeZoneM));
    dateTime = QDateTime::currentDateTime();
    ui->dateTimeEdit->setDateTime(dateTime);
    ui->timeLabel->setText(dateTime.toString(ui->dateTimeEdit->displayFormat()) + " (UTC)");
}

void NMEAClock::dateTimeChanged(){

    //if using threads, guard the local values with a lock
    dateTime = ui->dateTimeEdit->dateTime();
    ui->timeLabel->setText(dateTime.toString(ui->dateTimeEdit->displayFormat()) + " (UTC)");

    timeZoneH = ui->timeZoneHEdit->text().toInt();
    if(timeZoneH > 13)
        timeZoneH = 13;
    else if(timeZoneH < -13)
        timeZoneH = -13;
    timeZoneM = ui->timeZoneMEdit->text().toInt();
    if(timeZoneM > 59)
        timeZoneM = 59;
    else if (timeZoneM < -59)
        timeZoneM = -59;

    timeZoneString = formatTimeZoneH(timeZoneH) + "," + formatTimeZoneM(timeZoneH, timeZoneM);

    ui->timeZoneHEdit->setText(formatTimeZoneH(timeZoneH));
    ui->timeZoneMEdit->setText(formatTimeZoneM(timeZoneH, timeZoneM));
    //if using threads, unlock the lock here

    //start(); //update the NMEAString
    //labelTimer.stop();
}

QString NMEAClock::formatTimeZoneH(int hours){
    QString result;

    if(hours < 0){
        result = "-";
        hours *= -1;
    }else
        result = "+";

    if(hours < 10)
        result += "0";

    result += QString::number(hours);

    return result;
}

QString NMEAClock::formatTimeZoneM(int hours, int minutes){
    if(minutes < 0)
        minutes *= -1;

    QString result;

    if(hours < 0)
        result = "-";
    //else if(hours > 0)
    //    result = "+";

    if(minutes < 10)
        result += "0";

    result += QString::number(minutes);

    return result;
}

//parses the string, implement this in each plugin
void NMEAClock::parseNMEAString( const QString & text){
    //$--ZDA,hhmmss.ss,xx,xx,xxxx,xx,xx

    //if using threads, guard the local values with a lock

    QStringList pList = text.split(",");
    QString pDateTime = (QString)((QString)pList.at(1)).split(".").at(0) +
        pList.at(2) + pList.at(3) + pList.at(4);

    dateTime = QDateTime::fromString(pDateTime, "hhmmssddMMyyyy");

    ui->dateTimeEdit->setDateTime(dateTime);
    ui->timeLabel->setText(dateTime.toString(ui->dateTimeEdit->displayFormat()) + " (UTC)");
    timeZoneH = ((QString)pList.at(5)).toInt();
    timeZoneM = ((QString)pList.at(6)).toInt();

    timeZoneString = formatTimeZoneH(timeZoneH) + "," + formatTimeZoneM(timeZoneH, timeZoneM);

    ui->timeZoneHEdit->setText(formatTimeZoneH(timeZoneH));
    ui->timeZoneMEdit->setText(formatTimeZoneM(timeZoneH, timeZoneM));

    //if using threads, unlock the lock here

}

void NMEAClock::updateLabel(){
    dateTime = dateTime.addSecs(1);
    ui->timeLabel->setText(dateTime.toString(ui->dateTimeEdit->displayFormat()) + " (UTC)");
}

//called by the timer, implement this in each plugin
void NMEAClock::updateNMEAString(){

    //if using threads, guard the local values with a lock
    QString newString =
            "$IIZDA," +
            dateTime.toString("hhmmss.00,dd,MM,yyyy") +
            "," +
            timeZoneString;
    newString = newString + "*" + generateChecksum(newString);
    if(newString != NMEAString){
        NMEAString = newString;
        emit newNMEAString(NMEAString);
    }
    //stop(); //update done, disable the timer to save resources
    //labelTimer.start(1000);
    updateLabel();

    //if using threads, unlock the lock here
}

void NMEAClock::setTimerDelay(int tDelay){
    //if(tDelay < updateDelay)
    //    tDelay = updateDelay;
    CorePlugin::setTimerDelay(tDelay);
}
