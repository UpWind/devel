#define QT_NO_DEBUG_OUTPUT
/**
 * \file polardiagram.cpp
 * \author Carles Escrig i Royo
 *
 * Copyright (C) 2010 by University of Oulu (Finland)
 *
 * This file is part of UpWind Project.
 *
 * PolarDiagram class is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PolarDiagram class is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PolarDiagram class.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "polardiagram.h"

PolarDiagram::PolarDiagram(){
    debug = DEBUG;
    if (debug)
        qDebug() << Q_FUNC_INFO << ": debug mode";
}

PolarDiagram::PolarDiagram( const PolarDiagram &other){
    name = other.name;
    TWA = other.TWA;
    diagram = other.diagram;
}

PolarDiagram & PolarDiagram::populate(){
    // FAIL SAFE POLAR DIAGRAM
    this->name = "generic";
    this->TWA = true;
    this->diagram.clear();
    this->diagram[6].insert(45.0, 1100.0);
    this->diagram[6].insert(145.0, 1000.0);
    this->diagram[10].insert(43.0, 950.0);
    this->diagram[10].insert(150.0, 700.0);
    this->diagram[16].insert(41.0, 800.0);
    this->diagram[16].insert(175.0, 500.0);
    return *this;
}

PolarDiagram & PolarDiagram::populateWithFinngulf36(){
    // FAIL SAFE POLAR DIAGRAM
    this->name = "generic";
    this->TWA = true;
    this->diagram.clear();
    this->diagram[6].insert(52, 710);
    this->diagram[6].insert(60, 650.1);
    this->diagram[6].insert(75, 606.8);
    this->diagram[6].insert(90, 604.7);
    this->diagram[6].insert(110, 619.7);
    this->diagram[6].insert(120, 665.3);
    this->diagram[6].insert(135, 768.1);
    this->diagram[6].insert(150, 915.8);

    this->diagram[8].insert(52, 605.1);
    this->diagram[8].insert(60, 573.2);
    this->diagram[8].insert(75, 550.1);
    this->diagram[8].insert(90, 540.5);
    this->diagram[8].insert(110, 548.9);
    this->diagram[8].insert(120, 565);
    this->diagram[8].insert(135, 613.1);
    this->diagram[8].insert(150, 723.7);

    this->diagram[10].insert(52, 547.1);
    this->diagram[10].insert(60, 546.7);
    this->diagram[10].insert(75, 521.2);
    this->diagram[10].insert(90, 510.5);
    this->diagram[10].insert(110, 513.1);
    this->diagram[10].insert(120, 523.3);
    this->diagram[10].insert(135, 551.8);
    this->diagram[10].insert(150, 604.3);

    this->diagram[12].insert(52, 557.4);
    this->diagram[12].insert(60, 532.2);
    this->diagram[12].insert(75, 503.9);
    this->diagram[12].insert(90, 494.9);
    this->diagram[12].insert(110, 489.5);
    this->diagram[12].insert(120, 495.6);
    this->diagram[12].insert(135, 515.9);
    this->diagram[12].insert(150, 550.9);

    this->diagram[14].insert(52, 547.2);
    this->diagram[14].insert(60, 522.8);
    this->diagram[14].insert(75, 494.9);
    this->diagram[14].insert(90, 485.2);
    this->diagram[14].insert(110, 472.3);
    this->diagram[14].insert(120, 475.3);
    this->diagram[14].insert(135, 490.8);
    this->diagram[14].insert(150, 516.8);

    this->diagram[16].insert(52, 541);
    this->diagram[16].insert(60, 516.9);
    this->diagram[16].insert(75, 488.8);
    this->diagram[16].insert(90, 474.5);
    this->diagram[16].insert(110, 458.6);
    this->diagram[16].insert(120, 457.3);
    this->diagram[16].insert(135, 470.8);
    this->diagram[16].insert(150, 492.2);

    this->diagram[20].insert(52, 537.3);
    this->diagram[20].insert(60, 512);
    this->diagram[20].insert(75, 481.8);
    this->diagram[20].insert(90, 462.3);
    this->diagram[20].insert(110, 442.1);
    this->diagram[20].insert(120, 430.3);
    this->diagram[20].insert(135, 435.1);
    this->diagram[20].insert(150, 453.8);
    return *this;
}

void PolarDiagram::clear(){
    name.clear();
    diagram.clear();
}

QString PolarDiagram::getName() const{
    if(debug)
        qDebug() << Q_FUNC_INFO;
    return name;
}

void PolarDiagram::setName(const QString &ename){
    if (debug)
        qDebug() << Q_FUNC_INFO;
    name = ename;
}

bool PolarDiagram::getTWA() const{
    return isTWA();
}

bool PolarDiagram::isTWA() const{
    if(debug)
        qDebug() << Q_FUNC_INFO;
    return TWA;
}

void PolarDiagram::setTWA(bool istwa){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    TWA = istwa;
}

Diagram PolarDiagram::getDiagram(){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    return diagram;
}

void PolarDiagram::setDiagram(const Diagram &ediagram){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    diagram = ediagram;
}

Angles PolarDiagram::getAngles(const float &speed){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    if(contains(speed))
        return diagram.value(speed);
    else
        return Angles();
}

void PolarDiagram::setAngles(const float &speed, const Angles &angles){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    if(contains(speed)){
        Angles merge = angles;
        Angles aux = getAngles(speed);

        Angles::const_iterator i = aux.constBegin();
        while(i != aux.constEnd()){
            merge.insert(i.key(), i.value());
            ++i;
        }

        diagram.insert(speed, merge);

    } else
        diagram.insert( speed, angles);
}

QList<float> PolarDiagram::getSpeeds(){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    return diagram.keys();
}

int PolarDiagram::operator==(PolarDiagram &other){
    if(debug){
        QString res = (name == other.name &&
                       TWA == other.TWA &&
                       diagram == other.diagram) ? "true" : "false";
        qDebug() << Q_FUNC_INFO << QString(": %1").arg(res);
    }

    return(name == other.name &&
           TWA == other.TWA &&
           diagram == other.diagram);
}

int PolarDiagram::operator!=( PolarDiagram &other){
    if (debug) {
        QString res = ( name != other.name ||
                        TWA != other.TWA ||
                        diagram != other.diagram ) ? "true" : "false" ;
        qDebug() << QString("PolarDiagram::operator!=(): %1").arg( res);
    }

    return ( name != other.name ||
             TWA != other.TWA ||
             diagram != other.diagram );
}

UP_ERROR PolarDiagram::check(){
    if(debug)
        qDebug() << Q_FUNC_INFO;

    if(name.isEmpty())
        return ERROR_PD_NAME;
    if(diagram.size() == 0)
        return ERROR_PD_SIZE;

    QList<float> speeds = diagram.keys();

    for(int i = 0; i < speeds.size(); i++){
        // for each speed we check the list of angles-ta
        // very simple not in deep check of the diagram
        Angles aux = getAngles(speeds[i]);
        QList<float> angles = aux.keys();

        bool a_gybe_angle = false;
        bool a_beat_angle = false;
        float ang, ta;

        for(int j = 0; j < angles.size(); j++){
            ang = angles[j];
            ta = aux.value(ang);

            if(ang < MAX_ANGLE_CONSIDERED_BEAT)
                a_beat_angle = true;
            else if(ang > MIN_ANGLE_CONSIDERED_GYBE)
                a_gybe_angle = true;
            // just check if TA is a reasonable value
            if(ta < MIN_TA_ALLOWED || ta > MAX_TA_ALLOWED)
                return ERROR_PD_TA;
        }
        // at least one beat angle and one gybe angle
        // is required in each list of angles-ta
        if(!a_beat_angle || !a_gybe_angle)
            return ERROR_PD_ANG;
    }
    return ERROR_NONE;
}

bool PolarDiagram::isValid(){
    return(check() == ERROR_NONE);
}

bool PolarDiagram::contains(const float &speed){
    if(debug)
        qDebug() << Q_FUNC_INFO;
    return diagram.contains(speed);
}

float PolarDiagram::getGybeAngle(const float & speed){
    if(!contains(speed)){
        if(debug)
            qDebug() << Q_FUNC_INFO << QString("(%1): speed not found").arg(QString::number(speed));

        int i = 0;
        // List of all the speeds available in the diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while(speeds[i] < speed && i < speeds.size() - 1)
            ++i;

        float maxang;

        // input speed is bigger than any available
        if(i == speeds.size() - 1)
            maxang = getGybeAngle(speeds.last());
        // input speed is smaller than any available
        else if (i == 0)
            maxang = getGybeAngle(speeds.first());
        // input speed is between other available speeds
        else
            maxang = (getGybeAngle(speeds[i-1]) + getGybeAngle(speeds[i])) / 2;

        return maxang;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles(speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    float maxta = 0, maxang = 0, ta, ang;
    bool on_gybe = true;

    for(int i = angles.size() - 1; on_gybe && i >= 0; --i){
        ang = angles[i];
        ta = aux.value(ang);

        if(ang < MIN_ANGLE_CONSIDERED_GYBE)
            on_gybe = false;
        else
            if(ta > maxta){
                maxta = ta;
                maxang = ang;
            }
    }

    if(debug)
        qDebug() << Q_FUNC_INFO << QString("(%1): %2").arg(QString::number(speed), QString::number(maxang));

    return maxang;
}

float PolarDiagram::getBeatAngle( const float & speed){
    if(!contains(speed)){
        if(debug)
            qDebug() << Q_FUNC_INFO << QString("(%1): speed not found").arg(QString::number(speed));

        int i = 0;
        // List of ALL the speeds avilable in de diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while(speeds[i] < speed && i < speeds.size() - 1)
            ++i;

        float maxang;

        // input speed is bigger than any available
        if(i == speeds.size() - 1)
            maxang = getBeatAngle(speeds.last());
        // input speed is smaller than any available
        else if (i == 0)
            maxang = getBeatAngle(speeds.first());
        // input speed is between other available speeds
        else
            maxang = (getBeatAngle(speeds[i-1]) + getBeatAngle(speeds[i])) / 2;

        return maxang;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles(speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    float maxta = 0, maxang = 0, ta, ang;
    bool on_beat = true;

    for(int i = 0; on_beat && i < angles.size(); i++ ){
        ang = angles[i];
        ta = aux.value( ang);

        if(ang > MAX_ANGLE_CONSIDERED_BEAT)
            on_beat = false;
        else
            if(ta > maxta){
                maxta = ta;
                maxang = ang;
            }
    }
    if(debug)
        qDebug() << Q_FUNC_INFO << QString("(%1): %2").arg(QString::number(speed), QString::number(maxang));
    return maxang;
}

float PolarDiagram::getTA(const float & speed, const float & angle){
    Q_ASSERT(angle >= 0.0);
    if(!contains(speed)){
        int i = 0;
        // List of ALL the speeds avilable in de diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while(speeds.at(i) < speed && i < speeds.size() - 1)
            ++i;

        float ta;

        // input speed is bigger than any available
        if(i == speeds.size() - 1)
            ta = getTA(speeds.last(), angle);
        // input speed is smaller than any available
        else if(i == 0)
            ta = getTA(speeds.first(), angle);
        // input speed is between other available speeds
        else
            ta = (getTA(speeds.at(i - 1), angle) + getTA(speeds.at(i), angle)) / 2;

        return ta;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles(speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    int i = 0;

    while(angles.at(i) < angle && i < angles.size() - 1)
        ++i;

    // input angle is bigger than any available
    if(i == angles.size() - 1)
        return aux.value(angles.last());
    // input angle is smaller than any available
    else if(i == 0) {
        float zeroSpeed = 3600; // 0.1 knot speed
        float smallestSpeed = aux.value(angles.first());
        float smallestAngle = angles.first();
        // note, can't be zero as it wouldn't enter here in that case.
        return (smallestSpeed - zeroSpeed) / (smallestAngle) * angle + zeroSpeed;
//        return aux.value(angles.first());
    }
    // input angle is between other available angles
    else
        return (aux.value(angles.at(i - 1)) + aux.value(angles.at(i))) / 2;
}

float PolarDiagram::getAngle(const float & trueWindSpeed, const float & trueWindAngle){
    if(debug)
        qDebug() << Q_FUNC_INFO << QString("(%1, %2)").arg(QString::number(trueWindSpeed), QString::number(trueWindAngle));

    float TWA = fabs(trueWindAngle);
    char side = (trueWindAngle < 0) ? 'L' : 'R';
    float angle;

    // BEATING
    if(TWA <= 90 && TWA >= 0){
        angle = getBeatAngle(trueWindSpeed);

        if(TWA > angle){
            if(debug)
                qDebug() << QString("[UPWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number(TWA), QString::number(side), QString::number(trueWindSpeed), "reach");
            return 0;
        } else{
            if(debug)
                qDebug() << QString("[UPWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number(TWA), QString::number(side), QString::number(trueWindSpeed), QString::number(angle));
            return angle;
        }

        // RUNNING
    } else if(TWA > 90 && TWA <= 180){
        angle = getGybeAngle( trueWindSpeed);

        if(TWA < angle){
            if(debug)
                qDebug() << QString("[DOWNWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number(TWA), QString::number(side), QString::number(trueWindSpeed), "reach");
            return 0;
        } else{
            if(debug)
                qDebug() << QString("[DOWNWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number(TWA), QString::number(side), QString::number(trueWindSpeed), QString::number(angle));
            return angle;
        }
    } else{
        if(debug)
            qDebug() << QString("[UNKNOWN] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number(TWA), QString::number(side), QString::number(trueWindSpeed), "reach");
        return 0;
    }
}

void PolarDiagram::addLine( const float &speed, const float &angle, const float &TA){
    if(debug)
        qDebug() << Q_FUNC_INFO;

    Angles aux;
    aux.insert(angle, TA);

    // setAngles() will take care if the speed already exists
    setAngles(speed, aux);
}

PolarDiagram & PolarDiagram::operator =(PolarDiagram other){
    name = other.name;
    TWA = other.TWA;
    diagram = other.diagram;
    return *this;
}
