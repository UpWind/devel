#include "nmeavarwindspeedmeter.h"
#include "ui_nmeavarwindspeedmeter.h"
#include "settingsmanager.h"
//#include "dialog.h"
#include <QtWidgets>
#include <QDebug>
const static double KNOT_TO_METER_MULTI = 0.514444444;

NMEAVarWindSpeedMeter::NMEAVarWindSpeedMeter():
    ui(new Ui::NMEAVarWindSpeedMeter)
{
    qDebug() << __PRETTY_FUNCTION__;

    this->pluginName = "NMEA Var WindSpeed Meter";
    this->produceIDs = "MWD";
    this->parseIDs = "MWD";

    //autobuttonpressed = false;
    updatewindspeed=false;
    updatewindangle=false;
    CreateGUI();
    setTimerDelay(1000);
    plugin_widget->setWindowTitle(pluginName);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
    win= new MainWindow();
    connect(win,SIGNAL(passToPlugin(qreal)),this,SLOT(receiveWindSpeed(qreal)));
    connect(win,SIGNAL(SendDialToPlugin(QDial*)),this,SLOT(receiveDialValue(QDial*)));
    //win->setGeometry(100,100,700,700);
  //  win->setFixedHeight(700);
//    win->setFixedWidth(800);

    //win->setSizeIncrement();
    //win->setFixedSize(500,500);
    userinput=0;

    //dia = new Dialog();
    //  connect(dia,SIGNAL(valuesReady(QList<double>)),this,SLOT(valuesReady(QList<double>)));
}

NMEAVarWindSpeedMeter::~NMEAVarWindSpeedMeter(){
    delete ui;
    //delete allocated variables
}

void NMEAVarWindSpeedMeter::CreateGUI(){
    ui->setupUi(plugin_widget);
}

void NMEAVarWindSpeedMeter::parseNMEAString( const QString & text){
    //Parse the text
}

void NMEAVarWindSpeedMeter::sendDatatoPlugin(QList<double> argwindspeeds)
{
    qDebug() << __PRETTY_FUNCTION__;
    //autobuttonpressed = true;
    windspeeds = argwindspeeds;
}

void NMEAVarWindSpeedMeter::check()
{
    qDebug() << __PRETTY_FUNCTION__;
    //qDebug() << autobuttonpressed;
}

void NMEAVarWindSpeedMeter::updateNMEAString(){

    qDebug() << __PRETTY_FUNCTION__;
    qDebug() <<" updatewindspeed"<<updatewindspeed;

    qDebug()<<"entered NMEAVarWindSpeedMeter::updateNMEAString()";

    //This means that we did not get the updated wind speed and angle,
    //  it might be starting phase , before user clicked start timer button
    // or between that 5 sec tick timer
    if(updatewindspeed == false && updatewindangle == false)
    {
        QString string;
        QString windangle;
        double speedinknots;
        double speedinmeters;
        //UserInput tells us if user has already clicked a windspeed value,then we
        //use the previous wind speed
        //this is for if user did not input the winspeed for eg.10 seconds, with the user input
        //we know that he has pressed already one windspeed, then we use the previous windspeed
        //this updatewindspeed is for only windspeed, if it is not updated then it is false
        if(userinput)
        {
            windangle = wind ;
            speedinknots = knotSpeed;
            speedinmeters = meterSpeed;

        }
        //or else we use 0
        else
        {
            windangle = "0";
            speedinknots=0.0;
            speedinmeters = 0.0;
        }


        string = "$IIMWD,";
        string += windangle + ".00,T," + windangle + ".00,M,";

        string += QString::number(speedinknots) + ",N,";
        string += QString::number(speedinmeters) + ",M";
        string += "*" + this->generateChecksum(string);

        qDebug()<<"nmea str"<<string;

        emit newNMEAString(string);

    }
    else//if it comes here, we have received either updated windspeed or updated windangle
    {
        userinput=1;
        QString str;

        if(updatewindangle)//if we receive updated wind angle , access that
        {
            //This means that user has windspeed set for the time period,missed dial value
            if(updateddial->value()==0)
                //if(win->getDialValue()->value()==0)
            {
                ///use the previous one
            }
            else
            {
                //wind = getInvertedDialValue(win->getDialValue());
                wind = getInvertedDialValue(updateddial);
            }
        updatewindangle=false;
        }
        else
        {
            //use the previous wind value
        }

        if(updatewindspeed)//if we get the updated wind speed, then update
        {
        knotSpeed = windspeed;//got windspeed from view
        updatewindspeed = false;
        }
        else
        {
            //use the previous wind knotspeed
        }
        meterSpeed = convertWindSpeed(knotSpeed);

        //$IIMWD,104.01,T,104.01,M,01.14,N,00.59,M*4C

        str = "$IIMWD,";
        str += wind + ".00,T," + wind + ".00,M,";

        str += QString::number(knotSpeed) + ",N,";
        str += QString::number(meterSpeed) + ",M";
        str += "*" + this->generateChecksum(str);

        qDebug()<<"nmea str"<<str;


        emit newNMEAString(str);
    }
}

void NMEAVarWindSpeedMeter::receiveWindSpeed(qreal windspeed)
{
    //qDebug()<<"plugin windspeed"<<windspeed;
    this->windspeed= windspeed;
    //qDebug()<<"this.windspeed"<<this->windspeed;
    updatewindspeed = true;

}

void NMEAVarWindSpeedMeter::receiveDialValue(QDial *dial)
{
    updateddial = dial;
    qDebug()<<"in plugin dial value is "<<updateddial->value();
    updatewindangle=true;
}

/*
If you need more info, check out the clock plugin. It's a simple NMEA plugin that contains a few extra comments
for new developers.
 */
QString NMEAVarWindSpeedMeter::getInvertedDialValue(QDial* dial)
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


float NMEAVarWindSpeedMeter::convertWindSpeed(int knots)
{
    int rounded; // Wind speed as an integer, with the excess decimals after the 2nd decimal place removed.
    float ms;    // Wind speed in meters per second.

    ms = knots*KNOT_TO_METER_MULTI;

    ms += 0.005;           // This guarantees that the number will be rounded up, instead of down.

    rounded = ms * 100;    // This calculation may seem pointless to you, but it will remove all
    ms = rounded / 100.00; // the decimals after the 2nd decimal place.

    return ms;
}

void NMEAVarWindSpeedMeter::on_pushButton_clicked()
{
}

void NMEAVarWindSpeedMeter::on_pushButton_2_clicked()
{
    //before showing the window we have to decide what lenght is the window
    win->passSimulationTimeToWindow(ui->spinBox->value());
win->setMinimumSize(700,700);
win->setMaximumSize(700,700);
    win->show();
}
