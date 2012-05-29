#include "nmea_anemometer.h"
#include <assert.h>
#include <QStringList>
#include <QString>
#include "ui_anemometerwidget.h"

const static double KNOT_TO_METER_MULTI = 0.514444444;

NMEA_Anemometer::NMEA_Anemometer():
        ui(new Ui::AnemometerWidget), windSpeedValue(0.0)
{
    CreateGUI();
    setTimerDelay(updateDelay);

    this->pluginName = "NMEA Windmeter";
    this->produceIDs = "MWD";
    this->parseIDs = "MWD";
    plugin_widget->setWindowTitle(pluginName);
}

NMEA_Anemometer::~NMEA_Anemometer()
{
    delete ui;
}

void NMEA_Anemometer::CreateGUI()
{
    ui->setupUi(plugin_widget);
    ui->windAngleDial->setStyle(&winStyle);

    QGridLayout *aLayout = (QGridLayout*)ui->frame->layout();
    aLayout->setAlignment(ui->windSpeedSlider, Qt::AlignCenter);

    connect(ui->windAngleDial, SIGNAL(valueChanged(int)), this, SLOT(on_windAngle_valueChanged()));
    connect(ui->windSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(on_windSpeed_valueChanged()));
}

void NMEA_Anemometer::on_windAngle_valueChanged()
{
    ui->windAngleValueLabel->setNum(getInvertedDialValue(ui->windAngleDial).toInt());
}

void NMEA_Anemometer::on_windSpeed_valueChanged()
{
    windSpeedValue = ui->windSpeedSlider->value();
    float ms; // Wind speed in meters per second
    QString speedString; // The wind speed should be printed in the following format: "x m/s y knots"
    QString valueString = QString::number(windSpeedValue);

    speedString += valueString;
    if (valueString.length() == 1)
        speedString += " "; /* Add indentation so that the string length remains constant.
                             * Apparently this doesn't really work like it should, because
                             * the wind speed is written in a variable-width font, which means
                             * that different characters may take up a different amount of space.
                             */
    speedString += " knots ";
    // Convert the wind speed from knots to m/s and round it up to 2 decimal places.
    ms = convertWindSpeed(windSpeedValue);

    valueString = QString::number(ms);
    speedString += valueString;
    speedString += " m/s";

    ui->windSpeedValueLabel->setText(speedString);
}

QString NMEA_Anemometer::getInvertedDialValue(QDial* dial)
{
    QString str;
    int val = dial->value();
    if (val > 180) {
        val -= 180;
    } else {
        val += 180;
    }
    if (val <100)
      str = "0";
    if (val <10)
      str += "0";

    str += QString::number(val);
    return str;
}

float NMEA_Anemometer::convertWindSpeed(int knots)
{
    int rounded; // Wind speed as an integer, with the excess decimals after the 2nd decimal place removed.
    float ms;    // Wind speed in meters per second.

    ms = knots*KNOT_TO_METER_MULTI;

    ms += 0.005;           // This guarantees that the number will be rounded up, instead of down.

    rounded = ms * 100;    // This calculation may seem pointless to you, but it will remove all
    ms = rounded / 100.00; // the decimals after the 2nd decimal place.

    return ms;
}

void NMEA_Anemometer::updateNMEAString()
{
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

    QString wind = getInvertedDialValue(ui->windAngleDial);

    QString str;
    double knotSpeed = windSpeedValue;
    double meterSpeed = convertWindSpeed(knotSpeed);

    /* //The speed calculation does not work by the way. This is commented as MWD is all we need.
    //$IIMWV,167,R,00.00,N,A*23

    str = "$IIMWV,";
    str += wind;
    str += ",T,";
    str += QString::number((int)windSpeedValue);
    str += ".0,";
    if(msButton->isChecked()){
       str += "K";
    }else if(milesButton->isChecked()){
       str += "M";
    }else if(knotsButton->isChecked()){
       str += "N";
    }

    // Validate the data
    str += ",A";

    str = str + "*" + generateChecksum(str);

    NMEAString = str;
    emit newNMEAString(NMEAString);*/

    //$IIMWD,104.01,T,104.01,M,01.14,N,00.59,M*4C
    str = "$IIMWD,";
    str += wind + ".00,T," + wind + ".00,M,";

    str += QString::number(knotSpeed) + ",N,";
    str += QString::number(meterSpeed) + ",M";
    str += "*" + this->generateChecksum(str);
    emit newNMEAString(str);
}

void NMEA_Anemometer::parseNMEAString(const QString & text)
{
    /*$IIMWD,316.00,T,316.00,M,11.2967,N,5.81152,M*44*/
    if(this->parseMessageID(text) == "MWV"){
        QStringList pList = text.split(",");
        QString angle = pList.at(1).split(".").at(0);
        // The "reference" field can be ignored, because its value is always "T"
        QString speed = pList.at(3).split(".").at(0);
        QString units = pList.at(4).split("*").at(0);

        // Update the wind angle dial's value.
        ui->windAngleDial->setValue((angle.toInt()+180)%360);
        // Also update the wind speed slider's value.
        ui->windSpeedSlider->setValue(speed.toInt());

        // We also need to update each label's text.
        ui->windAngleValueLabel->setText(QString((angle.toInt()+180)%360));
        ui->windSpeedValueLabel->setText(speed);

    }else if(this->parseMessageID(text) == "MWD"){
        QStringList pList = text.split(",");
        QString angle = pList.at(3).split(".").at(0);
        QString speed = pList.at(5);
        QString mspeed = pList.at(7);

        ui->windAngleDial->setValue((angle.toInt()+180)%360);
        // Also update the wind speed slider's value.
        ui->windSpeedSlider->setValue(speed.toInt());

        // We also need to update each label's text.
        ui->windAngleValueLabel->setText(angle);
        ui->windSpeedValueLabel->setText(QString::number(speed.toInt()) + " knots "+ mspeed +" m/s ");
        //msButton->setChecked(true);
    }
}

void NMEA_Anemometer::setTimerDelay(int tDelay){
    CorePlugin::setTimerDelay(updateDelay);
}
