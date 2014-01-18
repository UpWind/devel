#include "corenmeareader.h"
#include <QtGui>

#define DEBUG 1;

//another comment.

CoreNMEAReader::CoreNMEAReader(){
    debug = DEBUG;
    if(debug)
        qDebug() << "DataReader::DataReader()";

    // Setup buffers
    this->wsp_pos = 0;
    this->twa_pos = 0;
    this->hed_pos = 0;
    for(int i = 0; i < BUFFER_SIZE; i++){
        this->wsp[i] = NO_VALUE;
        this->twa[i] = NO_VALUE;
        this->hed[i] = NO_VALUE;
    }
    // TODO put initial values
}

QString CoreNMEAReader::generateChecksum(QString & nString){
    int i;
    quint8 xorRes = 0;
    int endI = nString.indexOf("*");
    if(endI < 0)
        endI = nString.length();
    for(i = 1; i< endI; i++)
        xorRes ^= nString.at(i).toLatin1();

    QString resString;
    resString.setNum(xorRes, 16);
    if(resString.length()==1)
        resString = "0" + resString;
    return resString;
}

void CoreNMEAReader::setRMC(QStringList RMC){
    this->RMC = RMC;//$GPRMC,040302.663,A,3939.7,N,10506.6,W,0.27,358.86,200804,,*1A
    this->RMCHour = RMC[1];//UTC Time: hhmmss.ss
    this->RMCStatus = RMC[2];//Status, V=Navigation receiver warning, A=Valid
    this->RMCLat = RMC[3];//Latitude,
    this->RMCLatDir = RMC[4];// N or S
    this->RMCLong = RMC[5];// Longitude,
    this->RMCLongDir = RMC[6];// E or W
    this->RMCSpeed = RMC[7];//Speed over ground, knots
    this->RMCTrackAngle = RMC[8];//Track made good, degrees true
    this->RMCDate = RMC[9];//Date, ddmmyy
    this->RMCMagVar = RMC[10];//Magnetic Variation, degrees
    getPosition();

    // Add heading to the buffer
    this->hed[hed_pos] = RMCTrackAngle.toFloat();
    hed_pos = ( hed_pos + 1 ) % BUFFER_SIZE;
}

float CoreNMEAReader::getAVGHeading() const{
    float total = 0, value;
    int count = 0;
    QString dbg;

    if(debug)
        dbg.append("DataReader::getAVGHeading(): ");

    for(int i = 0; i < BUFFER_SIZE; i++){
        value = this->hed[i];
        if (debug){
            dbg.append("[");
            dbg.append(QString::number(value));
            dbg.append("] ");
        }
        if(value != NO_VALUE){
            total = total + value;
            count++;
        }
    }
    if(debug){
        dbg.append(" total: ");
        dbg.append(QString::number(total));
        dbg.append(" count: ");
        dbg.append(QString::number(count));
        dbg.append(" AVG: ");
        dbg.append(QString::number(total / count));
        qDebug() << dbg;
    }
    return total / count;
}

qreal CoreNMEAReader::getLatitude(){
    qreal latitude = (qreal)getRMCLat().toDouble();
    qreal deg = latitude/100;
    int degrees = (int)deg;
    qreal min = deg-degrees;
    qreal latDecimal = degrees + min * 100 / 60;
    return latDecimal;
}

qreal CoreNMEAReader::getLongitude(){
    qreal longitude = (qreal)getRMCLong().toDouble();
    qreal deg2 = longitude / 100;
    int degrees2 = (int)deg2;
    qreal min2 = deg2 - degrees2;
    qreal longDecimal = degrees2 + min2 * 100 / 60;
    return longDecimal;
}

QPointF CoreNMEAReader::getPosition(){
    QPointF position;
    position.setY(this->getLatitude());
    position.setX(this->getLongitude());
    return position;
}

QString CoreNMEAReader::getRMCHour() const{
    return RMCHour;
}

QString CoreNMEAReader::getRMCStatus() const{
    return RMCStatus;
}

QString CoreNMEAReader::getRMCLat() const{
    return RMCLat;
}

QString CoreNMEAReader::getRMCLong() const{
    return RMCLong;
}

QString CoreNMEAReader::getRMCSpeed() const{
    return RMCSpeed;
}

float CoreNMEAReader::getRMCTrackAngle() const{
    return RMCTrackAngle.toFloat();
}

QString CoreNMEAReader::getRMCDate() const{
    return RMCDate;
}

QString CoreNMEAReader::getRMCMagVar() const{
    return RMCMagVar;
}

void CoreNMEAReader::setMWV(QStringList MWV){//Wind Speed and Angle
    this->MWV = MWV;//$IIMWV,273,R,24.3,N,A*10
    this->MWVWindAng = MWV[1]; //Wind angle, 0 to 360 degrees
    this->MWVRefer = MWV[2]; //Reference, R=Relative, T=True
    this->MWVWindSpeed = MWV[3]; //Wind Speed
    this->MWVUnits = MWV[4]; //Wind Speed units K/M/N
    this->MWVStatus = MWV[5].mid(0,1); //Status, A= Data Valid

    // Add wind speed to the buffer
    this->wsp[wsp_pos] = MWVWindSpeed.toFloat();
    wsp_pos = (wsp_pos + 1) % BUFFER_SIZE;
}

float CoreNMEAReader::getAVGWindSpeed() const{
    float total = 0, value;
    int count = 0;
    QString dbg;

    if(debug)
        dbg.append("DataReader::getAVGWindSpeed(): ");

    for(int i = 0; i < BUFFER_SIZE; i++){
        value = this->wsp[i];
        if(debug){
            dbg.append("[");
            dbg.append(QString::number(value));
            dbg.append("] ");
        }
        if(value != NO_VALUE){
            total = total + value;
            count++;
        }
    }
    if(debug){
        dbg.append(" total: ");
        dbg.append(QString::number( total));
        dbg.append(" count: ");
        dbg.append(QString::number( count));
        dbg.append(" AVG: ");
        dbg.append(QString::number( total / count));
        qDebug() << dbg;
    }
    return total / count;
}

double CoreNMEAReader::getMWVWindAng() const{
    return MWVWindAng.toDouble();
}

double CoreNMEAReader::getWindAngleRadians(){
    double windAngleRadians = (getMWVWindAng() * 2 * M_PI) / 360;
    return windAngleRadians;
}

QString CoreNMEAReader::getMWVRefer() const{
    return MWVRefer;
}

qreal CoreNMEAReader::getMWVWindSpeed() const{
    return (qreal)MWVWindSpeed.toFloat();
}

QString CoreNMEAReader::getMWVUnits() const{
    return MWVUnits;
}

QString CoreNMEAReader::getMWVStatus() const{
    return MWVStatus;
}

void CoreNMEAReader::setVWT(QStringList VWT){ //True Wind Speed and Angle
    this->VWT = VWT;//$IIVWT,088,L,24.7,N,12.6,M,,*2E
    this->VWTCalcuWindAng = VWT[1]; // x.xL: Calculated wind angle relative to the vessel, 0 to 180 deg,
    this->VWTCalcuWindDir = VWT[2]; //left/right L/R of vessel heading
    this->VWTCalcuWindSpeed = VWT[3]; //x.xN: Calculated wind speed,
    this->VWTCalcuWindSpeedUnit = VWT[4];//knots
    this->VWTWindSpeedMS = VWT[5]; //x.x: Wind speed m/s
    this->VWTWindSpeedKmH = VWT[7]; //x.x: Wind speed km/h

    // TRUE WIND ANGLE
    float TWA = ( this->VWTCalcuWindDir.toLatin1().at(0) == 'L' ) ?
                this->VWTCalcuWindAng.toFloat() * (-1) : this->VWTCalcuWindAng.toFloat();
    // add TWA to the buffer
    this->twa[twa_pos] = TWA;
    twa_pos = (twa_pos + 1) % BUFFER_SIZE;
}

float CoreNMEAReader::getAVGTWA() const{
    float total = 0, value;
    int count = 0;
    QString dbg;

    if(debug)
        dbg.append("DataReader::getAVGTWA(): ");

    for(int i = 0; i < BUFFER_SIZE; i++){
        value = this->twa[i];
        if(debug){
            dbg.append("[");
            dbg.append(QString::number(value));
            dbg.append("] ");
        }
        if(value != NO_VALUE ){
            total = total + value;
            count++;
        }
    }
    if(debug){
        dbg.append(" total: ");
        dbg.append(QString::number(total));
        dbg.append(" count: ");
        dbg.append(QString::number(count));
        dbg.append(" AVG: ");
        dbg.append(QString::number(total / count));
        qDebug() << dbg;
    }
    return total / count;
}

float CoreNMEAReader::getVWTCalcuWindAng() const{
    return VWTCalcuWindAng.toFloat();
}

int CoreNMEAReader::getVWTCalcuWindDir() const{
    return VWTCalcuWindDir.toLatin1().at(0);
}

QString CoreNMEAReader::getVWTCalcuWindSpeed() const{
    return VWTCalcuWindSpeed;
}

qreal CoreNMEAReader::getVWTWindSpeedMS() const{
    return (qreal)VWTWindSpeedMS.toFloat();
}

qreal CoreNMEAReader::getVWTWindSpeedKmH() const{
    return (qreal)VWTWindSpeedKmH.toFloat();
}

void CoreNMEAReader::setHDT(QStringList HDT){
    this->HDT = HDT; // Heading, True: $IIHDT,029,T
    this->HDTtrueHeading = HDT[1]; //Heading degrees true x.xT
}

QString CoreNMEAReader::getHDTtrueHeading() const{
    return HDTtrueHeading;
}

void CoreNMEAReader::setHDM(QStringList HDM){
    this->HDM = HDM; //ex. $IIHDM,235.,M
    this->HDMHeading = HDM[1]; // 235.M
    this->HDMMagnetic = HDM[2].mid(0,1);
}

QString CoreNMEAReader::getHDMHeading() const{
    return HDMHeading;
}

QString CoreNMEAReader::getHDMMagnetic() const{
    return HDMMagnetic;
}

void CoreNMEAReader::setVHW(QStringList VHW){//Water Speed and Heading
    this->VHW = VHW; //ex. $IIVHW,259.,T,237.,M,05.00,N,09.26,K
    this->VHWHeadingTrue = VHW[1]; //259.T: Heading 259 deg True
    this->VHWHeadingMag = VHW[3]; //237.M : Heading 237 deg mag
    this->VHWSpeedWaterKnots = VHW[5]; //05.00: Speed 5 knots through the water
    this->VHWSpeedKHP = VHW[7]; //09.26K: Speed 9.26 KPH
}

QString CoreNMEAReader::getVHWHeadingTrue() const{
    return VHWHeadingTrue;
}

QString CoreNMEAReader::getVHWHeadingMag() const{
    return VHWHeadingMag;
}

qreal CoreNMEAReader::getVHWSpeedWaterKnots() const{
    return (qreal)VHWSpeedWaterKnots.toFloat();
}

qreal CoreNMEAReader::getVHWSpeedKHP() const{
    return (qreal)VHWSpeedKHP.toFloat();
}

void CoreNMEAReader::setDPT(QStringList DPT){
    this->DPT = DPT;//$IIDPT,009.9,-1.2,*42
    this->DPTDepth = DPT[1]; //Depth, meters
    this->DPTOffset = DPT[2];//Offset from transducer,
}

QString CoreNMEAReader::getDPTDepth() const{
    return DPTDepth;
}

void CoreNMEAReader::run() {}

//settingsinterface

QString CoreNMEAReader::getName(){
    return QString("CoreNMEAReader");
}
void CoreNMEAReader::addPluginSettingsToLayout(QLayout *layout)
{
	(void)layout;
}

Settings* CoreNMEAReader::getSettings(){
    return settings;
}
