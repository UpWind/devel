#include "polardiagram.h"

PolarDiagram::PolarDiagram(){
    debug = DEBUGPOLARDIAGRAM;

    if (debug) qDebug() << "PolarDiagram::PolarDiagram(): debug mode";
}

PolarDiagram::PolarDiagram( const PolarDiagram &other){
    name = other.name;
    TWA = other.TWA;
    diagram = other.diagram;
    debug=DEBUGPOLARDIAGRAM;
}

PolarDiagram & PolarDiagram::populate(){
    // FAIL SAFE POLAR DIAGRAM
    this->name = "generic";
    this->TWA = true;
    this->diagram.clear();
    this->diagram[6].insert( 45.0, 1100.0);
    this->diagram[6].insert( 145.0, 1000.0);
    this->diagram[10].insert( 43.0, 950.0);
    this->diagram[10].insert( 150.0, 700.0);
    this->diagram[16].insert( 41.0, 800.0);
    this->diagram[16].insert( 175.0, 500.0);
    return *this;
}

void PolarDiagram::clear(){
    name.clear();
    diagram.clear();
}

QString PolarDiagram::getName() const{
    if (debug) qDebug() << "PolarDiagram::getName()";

    return name;
}

void PolarDiagram::setName( const QString &ename){
    if (debug) qDebug() << "PolarDiagram::setName()";

    name = ename;
}

bool PolarDiagram::getTWA() const{
    return isTWA();
}

bool PolarDiagram::isTWA() const{
    if (debug) qDebug() << "PolarDiagram::isTWA()";

    return TWA;
}

void PolarDiagram::setTWA( bool istwa){
    if (debug) qDebug() << "PolarDiagram::setTWA()";

    TWA = istwa;
}

Diagram PolarDiagram::getDiagram(){
    if (debug) qDebug() << "PolarDiagram::getDiagram()";

    return diagram;
}

void PolarDiagram::setDiagram( const Diagram &ediagram ){
    if (debug) qDebug() << "PolarDiagram::setDiagram()";

    diagram = ediagram;
}

Angles PolarDiagram::getAngles( const float &speed ){
    if (debug) qDebug() << "PolarDiagram::getAngles()";

    if ( contains( speed) )
            return diagram.value( speed);
    else
            return Angles();
}

void PolarDiagram::setAngles( const float &speed, const Angles &angles ){
    if (debug) qDebug() << "PolarDiagram::setAngles()";

    if ( contains( speed) ) {
        Angles merge = angles;

        Angles aux = getAngles( speed);

        Angles::const_iterator i = aux.constBegin();
        while (i != aux.constEnd()) {
                merge.insert( i.key(), i.value());
                ++i;
        }

        diagram.insert( speed, merge);

    } else {
        diagram.insert( speed, angles);
    }

}

QList<float> PolarDiagram::getSpeeds(){
    if (debug) qDebug() << "PolarDiagram::getSpeeds()";

    return diagram.keys();
}

int PolarDiagram::operator==( PolarDiagram &other){
    if (debug) {
        QString res = ( name == other.name && TWA == other.TWA && diagram == other.diagram ) ? "true" : "false" ;
        qDebug() << QString("PolarDiagram::operator==(): %1").arg( res);
    }

    return ( name == other.name && TWA == other.TWA && diagram == other.diagram );
}

int PolarDiagram::operator!=( PolarDiagram &other){
    if (debug) {
        QString res = ( name != other.name || TWA != other.TWA || diagram != other.diagram ) ? "true" : "false" ;
        qDebug() << QString("PolarDiagram::operator!=(): %1").arg( res);
    }

    return ( name != other.name || TWA != other.TWA || diagram != other.diagram );
}

UP_ERROR PolarDiagram::check(){
    if (debug) qDebug() << "PolarDiagram::check()";

    if ( name.isEmpty() )
        return ERROR_PD_NAME;

    if ( diagram.size() == 0)
        return ERROR_PD_SIZE;

    QList<float> speeds = diagram.keys();

    for ( int i = 0; i < speeds.size(); i++) {

        // for each speed we check the list of angles-ta
        // very simple not in deep check of the diagram
        Angles aux = getAngles( speeds[i]);
        QList<float> angles = aux.keys();

        bool a_gybe_angle = false;
        bool a_beat_angle = false;
        float ang, ta;

        for ( int j = 0; j < angles.size(); j++ ) {

            ang = angles[j];
            ta = aux.value( ang);

            if ( ang < MAX_ANGLE_CONSIDERED_BEAT )
                a_beat_angle = true;
            else if ( ang > MIN_ANGLE_CONSIDERED_GYBE )
                a_gybe_angle = true;
            // just check if TA is a reasonable value
            if ( ta < MIN_TA_ALLOWED || ta > MAX_TA_ALLOWED )
                return ERROR_PD_TA;
        }

        // at least one beat angle and one gybe angle
        // is required in each list of angles-ta
        if ( !a_beat_angle || !a_gybe_angle )
            return ERROR_PD_ANG;

    }

    return ERROR_NONE;
}

bool PolarDiagram::isValid(){
    return ( check() == ERROR_NONE );
}

bool PolarDiagram::contains( const float &speed){
    if (debug) qDebug() << "PolarDiagram::contains()";

    return diagram.contains( speed);
}

float PolarDiagram::getGybeAngle( const float & speed){
    if ( !contains( speed) ) {

        if (debug) qDebug() << QString("PolarDiagram::getGybeAngle(%1): speed not found").arg(QString::number( speed));

        int i = 0;
        // List of all the speeds available in the diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while ( speeds[i] < speed && i < speeds.size() -1) i++;

        float maxang;

        // input speed is bigger than any available
        if ( i == speeds.size()-1 && speeds[i]<speed )
            maxang = getGybeAngle( speeds.last());
        // input speed is smaller than any available
        else if ( i == 0 )
            maxang = getGybeAngle( speeds.first());
        // input speed is between other available speeds
        else
            maxang=interpolation(speeds[i-1],speeds[i],getGybeAngle( speeds[i-1]),getGybeAngle( speeds[i] ),speed);

        if (debug) qDebug() << QString("PolarDiagram::getGybeAngleModif(%1): %2").arg(QString::number( speed),QString::number( maxang));
        return maxang;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles( speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    float maxta = 0, maxang = 0, ta, ang,vmg=0;
    bool on_gybe = true;

    for ( int i = angles.size() - 1 ; on_gybe && i >= 0; i--) {

        ang = angles[i];

        ta = aux.value( ang);
        vmg=fabs((3600/ta)*cos( UwMath::toRadians(ang)));

        if ( ang < MIN_ANGLE_CONSIDERED_GYBE ) {
            on_gybe = false;

        } else {

            if (  vmg > maxta ) {
                maxta = vmg;
                maxang = ang;
            }
        }
    }

    if (debug) qDebug() << QString("PolarDiagram::getGybeAngle(%1): %2").arg(QString::number( speed),QString::number( maxang));

    return maxang;
}

float PolarDiagram::getBeatAngle( const float & speed){
    if ( !contains( speed) ) {

        if (debug) qDebug() << QString("PolarDiagram::getBeatAngle(%1): speed not found").arg(QString::number( speed));

        int i = 0;
        // List of ALL the speeds avilable in de diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while ( speeds[i] < speed && i < speeds.size()-1) i++;

        float maxang;

        // input speed is bigger than any available
        if ( i == speeds.size()-1 && speeds[i]<speed )
            maxang = getBeatAngle( speeds.last());
        // input speed is smaller than any available
        else if ( i == 0 )
            maxang = getBeatAngle( speeds.first());
        // input speed is between other available speeds
        else
            maxang=interpolation(speeds[i-1],speeds[i],getBeatAngle( speeds[i-1]),getBeatAngle( speeds[i] ),speed);

        if (debug) qDebug() << QString("PolarDiagram::getBeatAngleModif(%1): %2").arg(QString::number( speed),QString::number( maxang));
        return maxang;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles( speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    float maxta = 0, maxang = 0, ta, ang,vmg=0;
    bool on_beat = true;

    for ( int i = 0; on_beat && i < angles.size(); i++ ) {

        ang = angles[i];
        ta = aux.value( ang);

        vmg=fabs((3600/ta)*cos( UwMath::toRadians(ang)));

        if ( ang > MAX_ANGLE_CONSIDERED_BEAT ) {
        on_beat = false;

        } else {

            if (  vmg > maxta ) {
                maxta = vmg;
                maxang = ang;
            }
        }
    }

    if (debug) qDebug() << QString("PolarDiagram::getBeatAngle(%1): %2").arg(QString::number( speed),QString::number( maxang));

    return maxang;
}

float PolarDiagram::getVelocity( const float & speed, const float & angle){
    return 3600/getTA(speed,angle);
}

float PolarDiagram::getTA( const float & speed, const float & angle){
    if ( !contains( speed) ) {

        int i = 0;
        // List of ALL the speeds avilable in de diagram
        // given in ASC order
        QList<float> speeds = diagram.keys();

        while ( speeds.at( i) < speed && i < speeds.size() - 1) i++;

        float ta;

        // input speed is bigger than any available
        if ( i == speeds.size()-1 && speeds.at( i) < speed )
            ta = getTA( speeds.last(), angle);
        // input speed is smaller than any available
        else if ( i == 0 )
            ta = getTA( speeds.first(), angle);
        // input speed is between other available speeds
        else
            ta=interpolation(speeds.at( i-1),speeds.at( i),getTA( speeds.at( i-1), angle ),getTA( speeds.at( i), angle ),speed);

        if (debug) qDebug() << QString("PolarDiagram::getTA(%1,%2): %3").arg(QString::number( speed),QString::number( angle),QString::number( ta));
        return ta;
    }

    // input speed is contained in our diagram
    Angles aux = getAngles( speed);

    // List of available angles for this speed
    // List is in ASC order
    QList<float> angles = aux.keys();

    int i = 0;

    while ( angles.at( i) < angle && i < angles.size() - 1) i++;

    float ta;

    // input angle is bigger than any available
    if (angles.at(i)==angle)
        ta=aux.value(angles.at(i));
    else if ( i == angles.size()-1 && angles.at( i) < angle )
        ta= aux.value( angles.last() );
    // input angle is smaller than any available
    else if ( i == 0 )
        ta= aux.value( angles.first() );
    // input angle is between other available angles
    else
        ta= interpolation(angles.at( i-1),angles.at( i),aux.value( angles.at( i-1) ),aux.value( angles.at( i) ) ,angle);

    if (debug) qDebug() << QString("PolarDiagram::getTAInterpolate(%1,%2): %3").arg(QString::number( speed),QString::number( angle),QString::number( ta));
    return ta;
}

float PolarDiagram::getAngle( const float & trueWindSpeed, const float & trueWindAngle){
    if (debug) qDebug() << QString("PolarDiagram::getAngle( %1, %2)").arg(QString::number( trueWindSpeed),QString::number( trueWindAngle));

    float TWA = fabs( trueWindAngle);
    char side = ( trueWindAngle < 0 ) ? 'L' : 'R' ;
    float angle;

    // BEATING
    if ( TWA <= 90 && TWA >= 0 ) {

        angle = getBeatAngle( trueWindSpeed);

        if ( TWA > angle ) {
            if (debug) qDebug() << QString("[UPWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number( TWA),QString::number( side),QString::number( trueWindSpeed),"reach");
            return 0;
        } else {
            if (debug) qDebug() << QString("[UPWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number( TWA),QString::number( side),QString::number( trueWindSpeed),QString::number( angle));
            return angle;
        }

    // RUNNING
    } else if ( TWA > 90 && TWA <= 180 ) {

        angle = getGybeAngle( trueWindSpeed);

        if ( TWA < angle ) {
            if (debug) qDebug() << QString("[DOWNWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number( TWA),QString::number( side),QString::number( trueWindSpeed),"reach");
            return 0;
        } else {
            if (debug) qDebug() << QString("[DOWNWIND] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number( TWA),QString::number( side),QString::number( trueWindSpeed),QString::number( angle));
            return angle;
        }

    // NOT VALID TWA
    } else {
        if (debug) qDebug() << QString("[UNKNOWN] TWA: %1 %2 Wind speed: %3 Output: %4").arg(QString::number( TWA),QString::number( side),QString::number( trueWindSpeed),"reach");
        return 0;
    }
}

void PolarDiagram::addLine( const float &speed, const float &angle, const float &TA){
    if (debug) qDebug() << "PolarDiagram::addLine()";

    Angles aux;
    aux.insert( angle, TA);

    // setAngles() will take care if the speed already exists
    setAngles( speed, aux);
}

PolarDiagram & PolarDiagram::operator = ( PolarDiagram other){
    name = other.name;
    TWA = other.TWA;
    diagram = other.diagram;

    return *this;
}

float PolarDiagram::interpolation( float x1,float x2,float y1, float y2, float x){

    if((x2-x1)==0) return y2;//y2 should be equal to y1
    else return (((x-x1)/(x2-x1))*(y2-y1)+y1);

}


