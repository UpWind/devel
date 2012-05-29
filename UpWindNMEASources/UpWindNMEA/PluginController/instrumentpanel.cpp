#include "instrumentpanel.h"
#include "ui_instrumentpanel.h"
#include "math.h"

#include "QDebug"

static const double PI = 3.141592;

InstrumentPanel::InstrumentPanel(QWidget *parent):
    QWidget(parent),
    ui(new Ui::InstrumentPanel),
    min(":/images/minimize.png"),
    max(":/images/maximize.png"),
    trueWindAngle(0),
    trueWindSpeed(0),
    appWindAngle(0),
    appWindSpeed(0) {

    ui->setupUi(this);
    ui->maxMinButton->setIcon(max);
    ui->panelFrame->setHidden(true);

    connect(ui->maxMinButton, SIGNAL(clicked()), this, SLOT(hidePanel()));

}

InstrumentPanel::~InstrumentPanel() {

    delete ui;

}

void InstrumentPanel::parseNmeaString(const QString &text) {

    QStringList tokens;

    //Time information
    //$IIZDA,084739.00,18,11,2010,+01,00*50
    if(text.contains("ZDA")) {
        tokens = text.split(",");
        QString hoursMinutesSeconds(tokens.at(1));
        QString hours(hoursMinutesSeconds.left(2));
        QString minutes(hoursMinutesSeconds.mid(2, 2));
        QString seconds(hoursMinutesSeconds.mid(4, 2));
        ui->timeLabel->setText(hours + ":" + minutes + ":" + seconds);
        ui->dateLabel->setText(tokens.at(2) + "." + tokens.at(3) + "." + tokens.at(4));
    }
    //GPS information
    //$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    else if(text.contains("RMC")) {
        tokens = text.split(",");
        ui->latitudeLabel->setText(tokens.at(3) + " N ");
        ui->longitudeLabel->setText(tokens.at(5) + " E");
        boatSpeed = tokens.at(7).toDouble();
        ui->boatSpeedLabel->setText(QString::number(boatSpeed, 'f', 2) + " kn");
    }

    //Wind information
    //$IIMWD,348.51,T,348.51,M,00.00,N,00.00,M*44
    else if(text.contains("MWD")) {
        tokens = text.split(",");
        appWindAngle = tokens.at(1).toDouble();
        appWindSpeed = tokens.at(7).toDouble();
        ui->appWindAngleLabel->setText(QString::number(appWindAngle, 'f', 0) + " deg");
        ui->appWindSpeedLabel->setText(QString::number(appWindSpeed, 'f', 2) + " m/s");
    }
    else if(text.contains("HDG")) {
        tokens = text.split(",");
        //No empty headings
        if(tokens.at(1).size() != 0) {
            boatAngle = tokens.at(1).toDouble();
            ui->headingLabel->setText(tokens.at(1) + " deg");
        }
    }

    double appWindSpeedKt = appWindSpeed / 0.514444444;

    ui->trueWindAngleLabel->setText((QString::number(getTrueWindAngle(boatSpeed, appWindSpeedKt,
                                                                      appWindAngle, boatAngle), 'f', 0)) + " deg");
    //We must show the True Wind Speed in m/s
    double trueWindSpeedMS = getTrueWindSpeed(boatSpeed, appWindSpeedKt, appWindAngle, boatAngle);
    trueWindSpeedMS *= 0.514444444;
    ui->trueWindSpeedLabel->setText((QString::number(trueWindSpeedMS, 'f', 2)) + " m/s");

    //Calculate true wind speed and angle
    //static const double knotsToMeters = 0.514444444;
    //static const double metersToKnot = 1.0 / 0.514444444;


}

void InstrumentPanel::hidePanel() {

    if(ui->panelFrame->isHidden()) {
        ui->maxMinButton->setIcon(min);
        ui->panelFrame->show();
    }
    else {
        ui->maxMinButton->setIcon(max);
        ui->panelFrame->hide();
    }
}

double InstrumentPanel::getTrueWindSpeed(double boatSpeed, double apparentWindSpeed, double windDirection, double boatDirection) {
    double trueWindSpeed = 0;
    double angle = boatDirection - windDirection;

    if (angle < 0)
        angle = angle *(-1);
    if(angle > 180)
        angle = 180-(angle-180);
    if (apparentWindSpeed != 0 && angle != 0){
        double Y = 90 - angle;
        //to radians
        Y = Y*PI/180;
        double a = apparentWindSpeed * cos(Y);
        double bb = apparentWindSpeed * sin(Y);
        double b = bb - boatSpeed;
        trueWindSpeed = sqrt((a*a)+(b*b));
    }
    return trueWindSpeed;
}

double InstrumentPanel::getTrueWindAngle(double boatSpeed, double apparentWindSpeed, double windDirection, double boatDirection)  {
    double trueWindAngle = 0;
    double angle = boatDirection - windDirection;

    if (angle < 0)
        angle = angle *(-1);
    if(angle > 180)
        angle = 180-(angle-180);
    if (apparentWindSpeed != 0 && angle != 0){
        double Y = 90 - angle;
        //to radians
        Y = Y*PI/180;
        double a = apparentWindSpeed * cos(Y);
        double bb = apparentWindSpeed * sin(Y);
        double b = bb - boatSpeed;
        double atang = (atan(b/a)*(180/PI));
        trueWindAngle = 90 - atang;
        if ((boatDirection-windDirection) < 0){
            if((boatDirection-windDirection) > -180){
                //add
                trueWindAngle = boatDirection + trueWindAngle;
                if(trueWindAngle > 360)
                    trueWindAngle -= 360;
            }else{
                //substract
                trueWindAngle = boatDirection - trueWindAngle;
                if (trueWindAngle < 0)
                    trueWindAngle += 360;
            }
        }else{
            if((boatDirection-windDirection) < 180){
                //substract
                trueWindAngle = boatDirection - trueWindAngle;
                if (trueWindAngle < 0)
                    trueWindAngle += 360;
            }else{
                //add
                trueWindAngle = boatDirection + trueWindAngle;
                if(trueWindAngle > 360)
                    trueWindAngle -= 360;
            }
        }
    }
    return trueWindAngle;
}
