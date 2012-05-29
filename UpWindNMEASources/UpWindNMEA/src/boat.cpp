#include "boat.h"
#include <QMessageBox>
#include <QFileDialog>
#include "dms.h"
#include <math.h>
#include <QDebug>

Boat::Boat():
        latitude(0),longnitude(0),boatDirection(0),
        windDirection(0),windSpeed(0),boatSpeed(0),usingSpinnaker(false),timerMultiplier(1),isMoving(false),
        positionSelected(false), spinnakerAvailable(false)
{
}

int Boat::openFile(bool dialog){
    QDomDocument doc( "UpWindBoat" );
    QString oldFileName;
    if(dialog){
        oldFileName = fileName;
        fileName = QFileDialog::getOpenFileName(0, ("Open XML Polar Diagram"), "", ("XML Files (*.xml)"));
    }
    if(fileName == ""){
        //Cancel button pressed
        fileName = oldFileName;
        return 4;
    }else{
        QFile file(fileName);
        if( !file.open( QIODevice::ReadOnly ) ){
            fileName = "";
            return 1;
        }
        if( !doc.setContent( &file ) ){
            file.close();
            fileName = "";
            return 2;
        }
        file.close();

        QDomElement root = doc.documentElement();
        if( root.tagName() != "UpWindBoat" ){
            fileName = "";
            return 3;
        }
        element = root;
        return 0;
    }
}

double Boat::calculateBoatSpeed(double anemoSpeed, double anemoAngle, double boatAngle){
//    if(fileName == ""){
//        return -1;
//    }else{
        double compassAngle = boatAngle;
        double windAngle = anemoAngle;
        double windSpeed = anemoSpeed;

        double angle = compassAngle - windAngle;

        if (angle < 0)
            angle = angle *(-1);
        if(angle > 180)
            angle = 180-(angle-180);
        if(windSpeed > 20)
            windSpeed = 20;
        if(windSpeed >= 2)
            return interpolateData(windSpeed,angle);
        else
            return 0;
//    }
}

double Boat::interpolateData(double windSpeed, double angle){
    double interpolatedBoatSpeedLow = 0;
    double interpolatedBoatSpeedHigh = 0;
    double boatInterpolateSpeed = 0;

    if (angle == 0 || windSpeed < 2){
        boatInterpolateSpeed = 0;
    }else{
        get2LowerWindSpeed();
        get2UpperWindSpeed();
        getWindSpeed(windSpeed);
        if(windSpeed < lowerWindSpeed1){
            get2LowerAngles(lowerWindSpeed1);
            get2UpperAngles(lowerWindSpeed1);
            if(angle < lowerAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(lowerWindSpeed1,angle,lowerAngle1,lowerAngle2);
                get2LowerAngles(lowerWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateAngle(lowerWindSpeed2, angle, lowerAngle1, lowerAngle2);
                boatInterpolateSpeed = createLineExtrapolation(lowerWindSpeed1,lowerWindSpeed2,interpolatedBoatSpeedLow,interpolatedBoatSpeedHigh,windSpeed);
            }
            else if(angle > upperAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(lowerWindSpeed1,angle, upperAngle1, upperAngle2);
                get2UpperAngles(lowerWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateAngle(lowerWindSpeed2, angle, upperAngle1, upperAngle2);
                boatInterpolateSpeed = createLineExtrapolation(lowerWindSpeed1,lowerWindSpeed2,interpolatedBoatSpeedLow,interpolatedBoatSpeedHigh,windSpeed);
            }
            else{
                getAngles(lowerWindSpeed1,angle);
                interpolatedBoatSpeedLow = extrapolateWindSpeed(windSpeed,lowAngle,lowerWindSpeed1,lowerWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateWindSpeed(windSpeed,highAngle,lowerWindSpeed1,lowerWindSpeed2);
                boatInterpolateSpeed = interpolate(lowAngle,interpolatedBoatSpeedLow,highAngle,interpolatedBoatSpeedHigh,angle);
            }
        }
        else if(windSpeed > upperWindSpeed1){
            get2LowerAngles(upperWindSpeed1);
            get2UpperAngles(upperWindSpeed1);
            if(angle < lowerAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(upperWindSpeed1,angle,lowerAngle1,lowerAngle2);
                get2LowerAngles(upperWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateAngle(upperWindSpeed2,angle,lowerAngle1,lowerAngle2);
                boatInterpolateSpeed = createLineExtrapolation(upperWindSpeed1,upperWindSpeed2,interpolatedBoatSpeedLow,interpolatedBoatSpeedHigh,windSpeed);
            }
            else if(angle > upperAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(upperWindSpeed1,angle,upperAngle1,upperAngle2);
                get2UpperAngles(upperWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateAngle(upperWindSpeed2, angle, upperAngle1, upperAngle2);
                boatInterpolateSpeed = createLineExtrapolation(upperWindSpeed1,upperWindSpeed2,interpolatedBoatSpeedLow, interpolatedBoatSpeedHigh, windSpeed);
            }
            else{
                getAngles(upperWindSpeed1, angle);
                interpolatedBoatSpeedLow = extrapolateWindSpeed(windSpeed, lowAngle,upperWindSpeed1, upperWindSpeed2);
                interpolatedBoatSpeedHigh = extrapolateWindSpeed(windSpeed, highAngle,upperWindSpeed1, upperWindSpeed2);
                boatInterpolateSpeed = interpolate(lowAngle,interpolatedBoatSpeedLow,highAngle, interpolatedBoatSpeedHigh, angle);
            }
        }
        else{
            getWindSpeed(windSpeed);
            get2LowerAngles(lowWindSpeed);
            get2UpperAngles(lowWindSpeed);
            if(angle < lowerAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(lowWindSpeed, angle, lowerAngle1, lowerAngle2);
                get2LowerAngles(highWindSpeed);
                interpolatedBoatSpeedHigh = extrapolateAngle(highWindSpeed, angle, lowerAngle1, lowerAngle2);
                boatInterpolateSpeed = interpolate(lowWindSpeed,interpolatedBoatSpeedLow,highWindSpeed,interpolatedBoatSpeedHigh,windSpeed);
            }
            else if(angle > upperAngle1){
                interpolatedBoatSpeedLow = extrapolateAngle(lowWindSpeed, angle, upperAngle1, upperAngle2);
                get2UpperAngles(highWindSpeed);
                interpolatedBoatSpeedHigh = extrapolateAngle(highWindSpeed, angle, upperAngle1, upperAngle2);
                boatInterpolateSpeed = interpolate(lowWindSpeed,interpolatedBoatSpeedLow, highWindSpeed, interpolatedBoatSpeedHigh, windSpeed);
            }
            else{
                interpolatedBoatSpeedLow = getAngleValues(lowWindSpeed, angle);
                interpolatedBoatSpeedHigh = getAngleValues(highWindSpeed, angle);
                boatInterpolateSpeed = interpolate(lowWindSpeed,interpolatedBoatSpeedLow,highWindSpeed,interpolatedBoatSpeedHigh,windSpeed);
            }
        }
    }

    // speed = 1/(TA/3600)
    if(boatInterpolateSpeed != 0){
        boatInterpolateSpeed = 3600/boatInterpolateSpeed;
    }
    return boatInterpolateSpeed;
}

double Boat::extrapolateAngle(double windSpeed, double angle, double firstValue, double secondValue){
    QString boatSpeed1;
    QString boatSpeed2;
    QDomNode nodo;
    QDomElement e;
    int control1 = 0;
    int control2 = 0;

    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        control2 = 0;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == windSpeed){
                    nodo = e.firstChild();
                    while( !nodo.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = nodo.toElement();
                        if(e.attribute("angle","").toDouble() == firstValue){
                            boatSpeed1 = e.attribute("TA","");
                        }
                        if(e.attribute("angle","").toDouble() == secondValue){
                            boatSpeed2 = e.attribute("TA","");
                        }
                        nodo = nodo.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control1 >= 1000 || control2 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes. MOVEMENT FORBIDDEN!");
        msgBox.exec();
        isMoving = false;
        return 0;
    }
    return createLineExtrapolation(firstValue,secondValue,boatSpeed1.toDouble(),boatSpeed2.toDouble(),angle);
}

double Boat::createLineExtrapolation(double value1, double value2, double boatSpeed1, double boatSpeed2, double neededValue){
    double m = ((boatSpeed2-boatSpeed1)/(value2-value1));
    double b = boatSpeed1 - (m * value1);
    return (m*neededValue + b);
}

void Boat::getWindSpeed(double windSpeed){
    QDomNode nodo;
    QDomElement e;
    int control = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    QString elem1 = "0";
    QString elem2 = "0";
    while(!nodo.isNull() && control < 1000){
        control ++;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(elem2 == elem1){
                    if(e.attribute("knots","").toDouble() <= windSpeed){
                        elem2 = e.attribute("knots","");
                    }
                    elem1 = e.attribute("knots","");
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes. MOVEMENT FORBIDDEN!");
        msgBox.exec();
        isMoving = false;
        return;
    }
    lowWindSpeed = elem2.toDouble();
    highWindSpeed = elem1.toDouble();
}

double Boat::getAngleValues(double windSpeed, double angle){
    QString angle1;
    QString boatSpeed1;
    QString boatSpeed2;
    QString angle2;
    QDomNode nodo;
    QDomElement e;
    int control1 = 0;
    int control2 = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        control2 = 0;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == windSpeed){
                    nodo = e.firstChild();
                    while( !nodo.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = nodo.toElement();
                        if( !e.isNull() ){
                            if( e.tagName() == "data" ){
                                if (angle1 == angle2){
                                    if(e.attribute("angle","").toDouble() <= angle){
                                        angle1 = e.attribute("angle","");
                                        boatSpeed1 = e.attribute("TA","");
                                    }
                                    angle2 = e.attribute("angle","");
                                    boatSpeed2 = e.attribute("TA","");
                                }
                            }
                        }
                        nodo = nodo.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control1 >= 1000 || control2 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes. MOVEMENT FORBIDDEN!");
        msgBox.exec();
        isMoving = false;
        return 0;
    }
    return interpolate(angle1.toDouble(), boatSpeed1.toDouble(),angle2.toDouble(),boatSpeed2.toDouble(),angle);
}

double Boat::interpolate(double value1, double boatSpeed1, double value2, double boatSpeed2, double value3){
    double boatSpeed;

    // this is made if the wind speed or the angle is equal than the
    // one stored in the XML file (no interpolation needed in that point
    if(boatSpeed1 == boatSpeed2){
        boatSpeed = boatSpeed1;
    }else{
        boatSpeed = (boatSpeed2-boatSpeed1)*(value3 - value1);
        boatSpeed = boatSpeed/(value2-value1);
        boatSpeed = boatSpeed + boatSpeed1;
    }
    return boatSpeed;
}

void Boat::get2LowerAngles(double windSpeed){
    QDomNode nodo;
    QDomElement e;
    lowerAngle1 = 1000;
    int control1 = 0;
    int control2 = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == windSpeed){
                    nodo = e.firstChild();
                    while( !nodo.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = nodo.toElement();
                        if( !e.isNull() ){
                            if( e.tagName() == "data" ){
                                if(lowerAngle1 >= e.attribute("angle","").toDouble()){
                                    lowerAngle2 = lowerAngle1;
                                    lowerAngle1 = e.attribute("angle","").toDouble();
                                }
                                if (lowerAngle2 > e.attribute("angle","").toDouble() && lowerAngle1 < e.attribute("angle","").toDouble()){
                                    lowerAngle2 = e.attribute("angle","").toDouble();
                                }
                            }
                        }
                        nodo = nodo.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control1 >= 1000 || control2 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes");
        msgBox.exec();
    }

}

void Boat::get2UpperAngles(double windSpeed){
    QDomNode nodo;
    QDomElement e;
    upperAngle1 = 0;
    int control1 = 0;
    int control2 = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        control2 = 0;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == windSpeed){
                    nodo = e.firstChild();
                    while( !nodo.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = nodo.toElement();
                        if( !e.isNull() ){
                            if( e.tagName() == "data" ){
                                if(upperAngle1 <= e.attribute("angle","").toDouble()){
                                    upperAngle2 = upperAngle1;
                                    upperAngle1 = e.attribute("angle","").toDouble();
                                }
                                if(upperAngle2 < e.attribute("angle","").toDouble() && upperAngle1 > e.attribute("angle","").toDouble()){
                                    upperAngle2 = e.attribute("angle","").toDouble();
                                }
                            }
                        }
                        nodo = nodo.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control1 >= 1000 || control2 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes");
        msgBox.exec();
    }
}

void Boat::get2LowerWindSpeed(){
    QDomNode nodo;
    QDomElement e;
    lowerWindSpeed1 = 1000;
    int control = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control < 1000){
        control ++;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(lowerWindSpeed1 >= e.attribute("knots","").toDouble()){
                    lowerWindSpeed2 = lowerWindSpeed1;
                    lowerWindSpeed1 = e.attribute("knots","").toDouble();
                }
                if (lowerWindSpeed2 > e.attribute("knots","").toDouble() && lowerWindSpeed1 < e.attribute("knots","").toDouble()){
                    lowerWindSpeed2 = e.attribute("knots","").toDouble();
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes");
        msgBox.exec();
    }

}

void Boat::get2UpperWindSpeed(){
    QDomNode nodo;
    QDomElement e;
    upperWindSpeed1 = 0;
    int control = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control < 1000){
        control ++;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(upperWindSpeed1 <= e.attribute("knots","").toDouble()){
                    upperWindSpeed2 = upperWindSpeed1;
                    upperWindSpeed1 = e.attribute("knots","").toDouble();
                }
                if(upperWindSpeed2 < e.attribute("knots","").toDouble() && upperWindSpeed1 > e.attribute("knots","").toDouble()){
                    upperWindSpeed2 = e.attribute("knots","").toDouble();
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes");
        msgBox.exec();
    }

}

double Boat::extrapolateWindSpeed(double windSpeed, double angle, double firstValue, double secondValue){
    QString boatSpeed1;
    QString boatSpeed2;
    QDomNode nodo;
    QDomNode intNode;
    QDomElement e;
    int control1 = 0;
    int control2 = 0;
    int control3 = 0;

    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        control2 = 0;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == firstValue){
                    intNode = e.firstChild();
                    while( !intNode.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = intNode.toElement();
                        if(e.attribute("angle","").toDouble() == angle){
                            boatSpeed1 = e.attribute("TA","");
                        }
                        intNode = intNode.nextSibling();
                    }
                }
                if(e.attribute("knots","").toDouble() == secondValue){
                    intNode = e.firstChild();
                    while( !intNode.isNull() && control3 < 1000){
                        control3 ++;
                        QDomElement e = intNode.toElement();
                        if(e.attribute("angle","").toDouble() == angle){
                            boatSpeed2 = e.attribute("TA","");
                        }
                        intNode = intNode.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    if(control1 >= 1000 || control2 >= 1000 || control3 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes. MOVEMENT FORBIDDEN!");
        msgBox.exec();
        isMoving = false;
        return 0;
    }
    return createLineExtrapolation(firstValue,secondValue,boatSpeed1.toDouble(),boatSpeed2.toDouble(),windSpeed);
}

void Boat::getAngles(double windSpeed, double angle){
    QString angle1;
    QString angle2;
    QDomNode nodo;
    QDomElement e;
    int control1 = 0;
    int control2 = 0;
    if( usingSpinnaker )
        nodo = tablaSpinnaker;
    else
        nodo = tabla;

    e = nodo.toElement();
    nodo = e.firstChild();

    while(!nodo.isNull() && control1 < 1000){
        control1 ++;
        control2 = 0;
        QDomElement e = nodo.toElement();
        if(!e.isNull()){
            if( e.tagName() == "speed" ){
                if(e.attribute("knots","").toDouble() == windSpeed){
                    nodo = e.firstChild();
                    while( !nodo.isNull() && control2 < 1000){
                        control2 ++;
                        QDomElement e = nodo.toElement();
                        if( !e.isNull() ){
                            if( e.tagName() == "data" ){
                                if (angle1 == angle2){
                                    if(e.attribute("angle","").toDouble() <= angle){
                                        angle1 = e.attribute("angle","");
                                    }
                                    angle2 = e.attribute("angle","");
                                }
                            }
                        }
                        nodo = nodo.nextSibling();
                    }
                }
            }
        }
        nodo = nodo.nextSibling();
    }
    lowAngle = angle1.toDouble();
    highAngle = angle2.toDouble();

    if(control1 >= 1000 || control2 >= 1000){
        QMessageBox msgBox;
        msgBox.setText("XML reading stopped after 1000 nodes. MOVEMENT FORBIDDEN!");
        msgBox.exec();
        isMoving = false;
        return;
    }
}

int Boat::calculateEndPoint() {

    if(!isMoving){
        return -1;
    }

    boatSpeed = calculateBoatSpeed(windSpeed, windDirection, boatDirection) * timerMultiplier;

    double dist =  (((boatSpeed / 3600.0) * (updateDelay/1000.0)) * 0.514444) / EARTHRADIUS;

    double brng = (boatDirection / 180) * PI;

    double lat = (latitude / 180) * PI;
    double lng = (longnitude / 180) * PI;

    double lat2 = asin( sin(lat)  *  cos(dist) + cos(lat)  *  sin(dist) * cos(brng) );
    double lon2 = lng + atan2(sin(brng) * sin(dist) * cos(lat),
                            cos(dist)-sin(lat) * sin(lat2));

    lat2 = (lat2 / PI) * 180;
    lon2 = (lon2 / PI) * 180;

    if (lon2 > 180){
        lon2 = -180 + (lon2 - 180);
    }

    if (lon2 < -180){
        lon2 = 180 + (lon2 + 180);
    }

    latitude = lat2;
    longnitude = lon2;
    return 0;
}

void Boat::setTimerMultiplier(int multiplier){
    timerMultiplier = multiplier;
}

//from degrees, minutes, seconds to decimals
double Boat::latDecimal( const QString & latstr) {

    double latdeg = latstr.left(2).toInt();
    double latmin = latstr.mid(2, 2).toInt();
    double latsec = latstr.right(3).toInt();

    return latdeg + latmin/60 + latsec/60000;
}

//from degrees, minutes, seconds to decimals
double Boat::longDecimal( const QString & lngstr) {

    double lngdeg = lngstr.left(3).toInt();
    double lngmin = lngstr.mid(3, 2).toInt();
    double lngsec = lngstr.right(3).toInt();

    return lngdeg + lngmin/60 + lngsec/60000;
}
